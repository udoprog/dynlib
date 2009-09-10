#include <stdio.h>
#include <string.h>

#include "dyn/store.h"
#include "dyn/btree.h"
#include "dyn/var.h"
  
int
main()
{
  dstore ds;
  btree tree;

  ds_init(&ds);
  btree_init(&ds, &tree);

  dvar test;
  v_integer(test) = 1001;
  
  int i = 0;
  char s[100];

  for (i = 0; i < 1000000; i++)
    {
      sprintf(s, "test%d", i);
      btree_insert(&tree, s, &test);
    }
  
  v_integer(test)++;
  
  //btree_printf(tree.root, 0);
  //printf("\n");
  //
  
  printf("memory usage = %zu\n", ds_size(&ds));
  
  printf("== %d \n", v_integer(*btree_find(&tree, "test1")->value));
  printf("== %d \n", v_integer(*btree_find(&tree, "test2")->value));
  printf("== %d \n", v_integer(*btree_find(&tree, "test3")->value));
  printf("== %d \n", v_integer(*btree_find(&tree, "test4")->value));
  
  ds_free(&ds);
  return 0;
}
