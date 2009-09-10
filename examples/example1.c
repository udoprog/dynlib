#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "dyn/store.h"

#define PARTIAL 4

void
_test_allocation()
{
  dstore ds;
  int grow = D_INITIAL_SIZE / PARTIAL;
  char *str;
  
  ds_init(&ds);
  
  str = ds_get(&ds, grow * PARTIAL);
  memset(str, 0xaa, grow * PARTIAL);
  assert(ds.d_ppos == 0);
  
  ds_get(&ds, grow * PARTIAL * 2);
  memset(str, 0xaa, grow * PARTIAL * 2);
  assert(ds.d_ppos == 1);
  
  ds_free(&ds);
}

void
_test_strings()
{
  char *str;
  int i = 0;
  int grow = D_INITIAL_SIZE / PARTIAL;
  
  dstore ds;
  
  ds_init(&ds);
  
  for (i = 0; i < PARTIAL; i++)
    {
      str = ds_get(&ds, grow);
      memset(str, 0x0, grow);
      assert(ds.d_ppos == (i + 1) / PARTIAL);
    }
  
  ds_free(&ds);
}

void
_test_uneven()
{
  int i = 0;
  char *str;
  
  dstore ds;
  
  ds_init(&ds);
  
  for (i = 0; i < D_INITIAL_SIZE; i++)
    {
      str = ds_get(&ds, D_INITIAL_SIZE - i);
      memset(str, 0x0, D_INITIAL_SIZE - i);
    }
  
  ds_free(&ds);
}

int main()
{
  printf("simple allocation...");
  _test_allocation();
  printf(" OK!\n");
  
  printf("strings...");
  _test_strings();
  printf(" OK!\n");

  printf("uneven...");
  _test_uneven();
  printf(" OK!\n");
}
