#ifndef _DYN_BTREE_
#define _DYN_BTREE_

#include "dyn/store.h"
#include "dyn/var.h"
#include "dyn/string.h"

#define repr(n)         (n == NULL ? "<null>" : "<int>")

#define BTREE_VALUE_TYPE dvar *
#define BTREE_KEY_TYPE   char *
#define BTREE_KEY_CMP(v1, v2, result) \
do { \
  result = strcmp(v1, v2); \
} while(0)

#define BTREE_COPY_KEY(to, from) \
do { \
  size_t l = strlen(from); \
  to = ds_str_get(bt->ds, sizeof(char) * l); \
  strncpy(to, from, l); \
  assert(to != NULL); \
} while (0)

typedef enum color_e {
  RED,
  BLACK
} color;

typedef struct node_s
{
  BTREE_VALUE_TYPE  value;
  BTREE_KEY_TYPE    key;
  struct node_s     *parent;
  struct node_s     *left;
  struct node_s     *right;
  color             color;
} node;

typedef struct btree_s
{
  struct node_s *root;
  dstore        *ds;
} btree;

void
btree_init(dstore *, btree *bt);

node *
btree_insert(btree *bt, BTREE_KEY_TYPE key, BTREE_VALUE_TYPE value);

node *
btree_find(btree *bt, BTREE_KEY_TYPE key);

void
btree_rotate_right(btree *, node *n);

void
btree_rotate_left(btree *, node *n);

unsigned int
btree_height(node *);

unsigned int
btree_count(node *);

void
btree_printf(node *, int);

#endif /* _DYN_BTREE_ */
