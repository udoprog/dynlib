#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "dyn/store.h"
#include "dyn/string.h"
#include "dyn/array.h"

#define PROTOTYPE "pt\xAA\xBB\xCC%d\xDD\xEE\xFF%d"
#define LIMIT     10000

/**
 * write unique string to a stream and quality check them later.
 */
void
_test_streams()
{
  int i = 0;
  char prototype[100];
  
  dstore ds;
  ds_init(&ds);
  
  dstream dss;
  ds_stream_init(&ds, &dss);
  
  size_t s_l = 0;
  
  for (i = 0; i < LIMIT; i++)
    {
      sprintf(prototype, PROTOTYPE, i, LIMIT - i);
      s_l = strlen(prototype);
      ds_stream_write(&dss, prototype, s_l);
    }
  
  char *pss_str = ds_stream_close(&dss);
  size_t pos = 0;
  
  for (i = 0; i < LIMIT; i++)
    {
      sprintf(prototype, PROTOTYPE, i, LIMIT - i);
      s_l = strlen(prototype);
      assert(memcmp(prototype, pss_str + pos, s_l) == 0);
      pos += s_l;
    }
  
  ds_free(&ds);
}

int main()
{
  _test_streams();
}
