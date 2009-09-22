#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "dyn/var.h"
#include "dyn/store.h"
#include "dyn/btree.h"

#define right_of(p, n)  (((p)->right) == (n))
#define left_of(p, n)   (((p)->left) == (n))
#define is_root(n)      ((n)->parent == NULL)
#define is_red(n)       (((n) != NULL) && (n)->color == RED)
#define is_black(n)     (((n) == NULL) || (n)->color == BLACK)
#define repr_color(n)   (is_red(n) ? 'R' : 'B')

node *
_btree_grandparent(node *n);

node *
_btree_uncle(node *n);

node *
_btree_sibling(node *n);

node *
_btree_node_alloc(btree *bt, BTREE_KEY_TYPE key, BTREE_VALUE_TYPE value);

void
btree_init(ds, bt)
  d_store      *ds;
  btree       *bt;
{
  bt->root = NULL;
  bt->ds   = ds;
}

node *
_btree_grandparent(n)
  node *n;
{
  if (n != NULL && n->parent != NULL)
    {
      return n->parent->parent;
    }
  
  return NULL;
}

node *
_btree_uncle(n)
  node *n;
{
  node *g = _btree_grandparent(n);

  if (g == NULL)
    return NULL;

  if (n->parent == g->left)
    return g->right;
  else
    return g->left;
}

node *
_btree_sibling(n)
  node *n;
{
  if (n->parent == NULL)
    return NULL;

  if (n->parent->left == n)
    return n->parent->right;
  else
    return n->parent->left;
}

void
btree_rotate_right(bt, n)
  btree *bt;
  node *n;
{
  node *left = n->left;

  // refuse to rotate if there is nothing to replace axis with.
  if (left == NULL) return;
  
  if (is_root(n))
    {
      bt->root      = left;
    }
  else
    {
      if (left_of(n->parent, n))  n->parent->left = left;
      else                        n->parent->right = left;
    }
  
  left->parent      = n->parent;
  n->parent         = left;
  n->left           = left->right;
  left->right       = n;
}

void
btree_rotate_left(bt, n)
  btree *bt;
  node *n;
{
  node *right = n->right;
  
  // refuse to rotate if there is nothing to replace axis with.
  if (right == NULL) {
    return;
  }
  
  if (is_root(n))
    {
      bt->root      = right;
    }
  else
    {
      if (left_of(n->parent, n))  n->parent->left = right;
      else                        n->parent->right = right;
    }
  
  right->parent     = n->parent;
  n->parent         = right;
  n->right          = right->left;
  right->left       = n;
}

node *
_btree_node_alloc(bt, key, value)
  btree       *bt;
  BTREE_KEY_TYPE    key;
  BTREE_VALUE_TYPE  value;
{
  node *n = d_store_get(bt->ds, sizeof(node));
  // assert we are not out of memory
  assert(n != NULL);
  
  BTREE_COPY_KEY(n->key, key);
  
  n->value  = value;
  n->left   = NULL;
  n->right  = NULL;
  n->parent = NULL;
  n->color  = RED;
  return n;
}

node *
btree_insert(bt, key, value)
  btree       *bt;
  BTREE_KEY_TYPE    key;
  BTREE_VALUE_TYPE  value;
{
  /* current (used in loop) */
  node *c = NULL;
  /* new */
  node *n = NULL;
  /* uncle */
  node *u = NULL;
  /* grandparent */
  node *g = NULL;
  
  /* cmp - compare between two nodes. */
  int cmp = 0;
  
  /** case 1: root node **/
  if (bt->root == NULL)
    {
      bt->root = _btree_node_alloc(bt, key, value);
      bt->root->color = BLACK;
      n = bt->root;
      goto insert_case1;
    }
  
  c = bt->root;
  while (1)
    {
      BTREE_KEY_CMP(c->key, key, cmp);
      
      if (cmp == 0)
        {
          n = c;
          break;
        }
      
      if (cmp > 0)
        {
          if (c->right == NULL)
            {
              c->right = _btree_node_alloc(bt, key, value);
              c->right->parent = c;
              n = c->right;
              break;
            }
          
          c = c->right;
          continue;
        }
      
      if (cmp < 0)
        {
          if (c->left == NULL)
            {
              c->left = _btree_node_alloc(bt, key, value);
              c->left->parent = c;
              n = c->left;
              break;
            }
          
          c = c->left;
          continue;
        }
    } /* end while */
  
  return n;
  assert(n!=NULL);
insert_case1:
  /* case 1: parent is NULL, which means this is the root node. */
  if (is_root(n))
  {
    n->color = BLACK;
    return n;
  }
  
  assert(n != NULL);

//insert_case2:
  /* case 2: parent is BLACK which is OK */
  if (is_black(n->parent))
  {
    return n;
  }
  
  assert(n != NULL);

//insert_case3:
  u = _btree_uncle(n);
  
  /* case 3:  */
  if (u != NULL && is_red(u))
    {
      n->parent->color = BLACK;
      u->color = BLACK;
      g = _btree_grandparent(n);
      g->color = RED;
      n = g;
      goto insert_case1;
    }

  assert(n != NULL);

//insert_case4:
  /* WTF?, read more */
  g = _btree_grandparent(n);
  assert(g!=NULL);
  
  if (right_of(n->parent, n) && left_of(g, n->parent))
    {
      btree_rotate_left(bt, n->parent);
      n = n->left;
    }
  else if (left_of(n->parent, n) && right_of(g, n->parent))
    {
      btree_rotate_right(bt, n->parent);
      n = n->right;
    }
  
  assert(n != NULL);

//insert_case5:
  g = _btree_grandparent(n);

  assert(g!=NULL);
  
  n->parent->color  = BLACK;
  g->color          = RED;
  
  if (left_of(n->parent, n) && (left_of(g, n->parent)))
    {
      btree_rotate_right(bt, g);
    }
  else
    {
      btree_rotate_left(bt, g);
    }
  
  assert(n != NULL);
  
  return n;
}

node *
btree_find(bt, key)
  btree           *bt;
  BTREE_KEY_TYPE  key;
{
  node *c = bt->root;
  
  if (c->left == NULL && c->right == NULL)
    {
      return c;
    }
  
  int cmp = 0;
  
  while (c != NULL)
    {
      BTREE_KEY_CMP(c->key, key, cmp);
      
      if (cmp == 0)
        {
          return c;
        }
      
      if (cmp > 0)
        {
          c = c->right;
          continue;
        }
      
      if (cmp < 0)
        {
          c = c->left;
          continue;
        }
    }
  
  return NULL;
}

unsigned int
btree_height(n)
  node *n;
{
  if (n == NULL)
    return 0;
  
  unsigned int c_l = btree_height(n->left),
               c_r = btree_height(n->right);
  
  return (c_l > c_r ? c_l : c_r) + 1;
}

unsigned int
btree_count(n)
  node *n;
{
  return (n == NULL ? 0 : btree_count(n->left) + btree_count(n->right) + 1);
}

void
btree_printf(n, depth)
  node *n;
  int depth;
{
  int i = 0;

  for (i = 0; i < depth; i++)
  {
    printf(" ");
  }
  
  printf("(%s %c\n", repr(n), repr_color(n));
  
  if (n->left != NULL)
  {
    btree_printf(n->left, depth + 2);
    printf("\n");
  }
  else
  {
    for (i = 0; i < depth + 2; i++)
    {
      printf(" ");
    }
    
    printf("L:null\n");
  }
  
  if (n->right != NULL)
  {
    btree_printf(n->right, depth + 2);
    printf("\n");
  }
  else
  {
    for (i = 0; i < depth + 2; i++)
    {
      printf(" ");
    }
    
    printf("R:null\n");
  }
  
  for (i = 0; i < depth; i++)
  {
    printf(" ");
  }
  
  printf(")");
}
