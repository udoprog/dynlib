#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "dyn/store.h"
#include "dyn/string.h"
#include "dyn/array.h"

int main()
  {
    char *s;
    int i;
    
    /*
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
    ds_stream_init(&ds, &dss);
    ds_stream_write(&dss, "test1", 5);
    ds_stream_write(&dss, "test2", 5);
    ds_stream_write(&dss, "test3", 5);
    
    int i = 0;
    for (i = 0; i < 1000; i++)
      ds_stream_write(&dss, "xxxxx", 5);
    
    printf("result=(%lu bytes)\n", ds_stream_size(&dss));
    ds_stream_close(&dss);

    ds_array da;
    
    ds_array_init(&ds, &da);
    
    i = 100;
    
    ds_array_append(&da, &i, Integer, 0);
    i++;
    ds_array_append(&da, &i, Integer, 0);
    i++;
    ds_array_append(&da, &i, Integer, 0);
    ds_array_append(&da, "test1", String, 5);
    i++;
    ds_array_append(&da, &i, Integer, 0);
    ds_array_append(&da, "test2", String, 5);
    ds_array_append(&da, "abcde", String, 5);
    double d = 1.01;
    ds_array_append(&da, &d, Decimal, 0);
    
    ds_array_item *current = NULL;
    
    ds_array_pop(&da);
    ds_array_pop(&da);
    ds_array_pop(&da);
    ds_array_pop(&da);
    
    ds_array_append(&da, "abcde fick", String, 10);
    printf("number of items in array = %d\n", ds_array_size(&da));
    
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
        case Decimal:
          printf("decimal = %f\n", i_decimal(current));
          break;
        }
      }
    
    printf("allocated size = %lu\n", ds_size(&ds));
    ds_free(&ds);


    */
    // just for fun, compare allocating 10000 strings to using this...
    
    time_t begin = 0;
    
    printf("testing strings...\n");
    begin = time(NULL);
    
    for (i = 0; i < 100000000; i++)
      {
        s = malloc(sizeof(char) * 17);
        memcpy(s, "abcdefabcdefabcd", 16);
        s[16] = '\0';
        free(s);
      }
    
    printf("malloc time = %lu\n", time(NULL) - begin);
    
    dstore ds_s;
    ds_init(&ds_s);
    
    printf("testing strings...\n");
    begin = time(NULL);
    
    for (i = 0; i < 100000000; i++)
      {
        s = ds_str_get(&ds_s, 16);
        memcpy(s, "abcdefabcdefabcd", 16);
      }
    
    printf("dynlib time = %lu\n", time(NULL) - begin);
    ds_free(&ds_s);
    
    sleep(10);
  }
