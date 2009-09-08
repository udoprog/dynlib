#ifndef _DYN_STRING_
#define _DYN_STRING_

#include "dyn/store.h"

typedef struct dstring_s
{
  /* pointer to the actual string */
  char    *s_pointer;
  /* the actual string length */
  int     s_length;
  /* the storage area for the string */
  dstore  *ds;
}
dstring;

char *
ds_str_get(dstore *, size_t n);

#endif /* _DYN_STRING_ */
