#include <sys/types.h>

#include "dyn/store.h"
#include "dyn/string.h"

char *
ds_str_get(ds, n)
dstore  *ds;
size_t n;
  {
    char *str_build = ds_get(ds, n+1);
    return str_build;
  }
