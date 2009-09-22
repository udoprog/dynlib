#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dyn/store.h"
#include "dyn/array.h"
#include "dyn/var.h"

#define PROTOTYPE "\xAA\xBB\xCC%d\xAA\xBB\xCC%d"
#define LIMIT 10000

void
_test_arrays()
{
  d_store ds;
  d_store_array da;
  d_store_array_item *current;
  int i;

  char prototype[100];
  
  d_store_init(&ds);
  d_store_array_init(&ds, &da);
  
  printf(" building array");
  fflush(stdin);
  for (i = 0; i < LIMIT; i++)
  {
    memset(prototype, 0x0, 100);
    sprintf(prototype, PROTOTYPE, i, LIMIT - i);
    d_store_array_append(&da, prototype, String, strlen(prototype));
  }
  
  printf(", checking array");
  fflush(stdin);
  i = 0;
  for_each_in(&da, current)
  {
    memset(prototype, 0x0, 100);
    sprintf(prototype, PROTOTYPE, i, LIMIT - i);
    assert(strcmp(i_string(current), prototype) == 0);
    ++i;
  }
  
  d_store_free(&ds);
}

int main()
{
  printf("Testing Arrays... ");
  fflush(stdin);
  _test_arrays();
  printf(", OK!\n");
}
