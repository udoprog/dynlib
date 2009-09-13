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
  dstore ds;
  ds_array da;
  ds_array_item *current;
  int i;

  char prototype[100];
  
  ds_init(&ds);
  ds_array_init(&ds, &da);
  
  printf(" building array");
  fflush(stdin);
  for (i = 0; i < LIMIT; i++)
  {
    memset(prototype, 0x0, 100);
    sprintf(prototype, PROTOTYPE, i, LIMIT - i);
    ds_array_append(&da, prototype, String, strlen(prototype));
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
  
  ds_free(&ds);
}

int main()
{
  printf("Testing Arrays... ");
  fflush(stdin);
  _test_arrays();
  printf(", OK!\n");
}
