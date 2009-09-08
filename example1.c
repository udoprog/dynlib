#include <string.h>
#include <stdio.h>

#include "dyn/store.h"

int main()
{
  dstore ds;
  ds_init(&ds);

  char *str = ds_get(&ds, 100);

  strcpy(str, "this is a test string");
  
  printf("str = %s\n", str);
  
  printf("memory usage (more is pre-allocated) = %d\n", ds_size(&ds));
  
  ds_free(&ds); // the string is freed here.
  
  dstore ds2;
  dstream dss;

  ds_init(&ds2);
  dss_init(&ds2, &dss);
  dss_write(&dss, "test1", 5);
  dss_write(&dss, "test2", 5);
  dss_write(&dss, "test3", 5);

  // must close
  char *final_string = dss_close(&dss);
  
  printf("final_string = %s\n", final_string);

  printf("memory usage (more is pre-allocated) = %d\n", ds_size(&ds2));
  
  ds_free(&ds2); // final_string is release here
}
