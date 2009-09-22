#include <sys/types.h>

#include "dyn/store.h"
#include "dyn/string.h"

char *
d_store_str_get(ds, n)
d_store  *ds;
size_t n;
  {
    char *str_build = d_store_get(ds, n+1);
    return str_build;
  }
