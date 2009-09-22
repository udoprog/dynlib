#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "dyn/store.h"
#include "dyn/btree.h"
#include "dyn/var.h"

#define NODES 10000000
#define KEY   "\xaa\xbb\xcc%d\xaa\xbb\xcc%d"
  
void
_test_storage()
{
  d_store ds;
  btree tree;

  d_store_init(&ds);
  btree_init(&ds, &tree);

  d_var test;
  v_integer(test) = 1001;
  
  int i = 0;
  char s[100];

  node *found = NULL;
  
  printf(" building");
  fflush(stdout);
  for (i = 0; i < NODES; i++)
    {
      sprintf(s, KEY, i, NODES - i);
      btree_insert(&tree, s, &test);
    }
  
  // increment the shared integer.
  v_integer(test)++;
  
  printf(", checking");
  fflush(stdout);
  for (i = 0; i < NODES; i++)
    {
      sprintf(s, KEY, i, NODES - i);
      found = btree_find(&tree, s);
      assert(found != NULL);
      assert(v_integer(*found->value) == 1002);
    }
  
  printf(", height = %u", btree_height(tree.root));
  printf(", count = %u", btree_count(tree.root));
  assert(btree_count(tree.root));
  
  d_store_free(&ds);
}

int
main()
{
  printf("Testing binary tree persitency (%d nodes, please wait)...", NODES);
  fflush(stdout);
  _test_storage();
  printf(", OK!\n");
  return 0;
}
