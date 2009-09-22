#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "dyn/store.h"

#define PARTIAL 4

void
_test_allocation()
{
  d_store ds;
  int grow = D_SIZE_INITIAL / PARTIAL;
  char *str;
  
  d_store_init(&ds);
  
  str = d_store_get(&ds, grow * PARTIAL);
  memset(str, 0xaa, grow * PARTIAL);
  assert(ds.d_ppos == 0);
  
  d_store_get(&ds, grow * PARTIAL * 2);
  memset(str, 0xaa, grow * PARTIAL * 2);
  assert(ds.d_ppos == 1);
  
  d_store_free(&ds);
}

void
_test_strings()
{
  char *str;
  int i = 0;
  int grow = D_SIZE_INITIAL / PARTIAL;
  
  d_store ds;
  
  d_store_init(&ds);
  
  for (i = 0; i < PARTIAL; i++)
    {
      str = d_store_get(&ds, grow);
      memset(str, 0x0, grow);
      assert(ds.d_ppos == (i + 1) / PARTIAL);
    }
  
  d_store_free(&ds);
}

void
_test_uneven()
{
  int i = 0;
  char *str;
  
  d_store ds;
  
  d_store_init(&ds);
  
  for (i = 0; i < D_SIZE_INITIAL; i++)
    {
      str = d_store_get(&ds, D_SIZE_INITIAL - i);
      memset(str, 0x0, D_SIZE_INITIAL - i);
    }
  
  d_store_free(&ds);
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
