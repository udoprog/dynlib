#include <stdio.h>
#include <string.h>

#include "dyn/store.h"
#include "dyn/string.h"
#include "dyn/array.h"

int main()
  {
    dstore ds;
    ds_init(&ds);
    
    int *int1 = ds_get(&ds, sizeof(int));
    int *int2 = ds_get(&ds, sizeof(int));
    
    printf("int1 = %d\n", *int1);
    printf("int2 = %d\n", *int2);
    
    char *str1 = ds_str_get(&ds, 12);
    
    strcpy(str1, "abcdefg");
    
    printf("str1 = %s\n", str1);
    
    dstream dss;
    dss_init(&ds, &dss);
    dss_write(&dss, "test1", 5);
    dss_write(&dss, "test2", 5);
    dss_write(&dss, "test3", 5);
    
    int i = 0;
    for (i = 0; i < 1000; i++)
      dss_write(&dss, "xxxxx", 5);
    
    printf("result=(%d bytes)\n", dss_size(&dss));
    dss_close(&dss);

    darray da;
    
    dsa_init(&ds, &da);
    
    i = 100;
    
    dsa_append(&da, &i, Integer, 0);
    i++;
    dsa_append(&da, &i, Integer, 0);
    i++;
    dsa_append(&da, &i, Integer, 0);
    dsa_append(&da, "test1", String, 5);
    i++;
    dsa_append(&da, &i, Integer, 0);
    dsa_append(&da, "test2", String, 5);
    dsa_append(&da, "abcde", String, 5);
    
    darray_item *current = NULL;
    
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    dsa_pop(&da);
    
    dsa_append(&da, "abcde fuck", String, 10);
    printf("number of items in array = %d\n", dsa_size(&da));
    
    for_each_in(&da, current)
      {
        switch (i_type(current))
        {
        case Integer:
          printf("integer = %d\n", i_integer(current));
          break;
        case String:
          printf("string = %s\n", i_string(current));
          break;
        }
      }
    
    printf("allocated size = %d\n", ds_size(&ds));
    ds_free(&ds);
  }
