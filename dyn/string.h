#ifndef _DYN_STRING_
#define _DYN_STRING_

#include "dyn/store.h"

typedef struct d_string_s
{
  /* pointer to the actual string */
  char    *s_pointer;
  /* the actual string length */
  int     s_length;
  /* the storage area for the string */
  d_store  *ds;
}
d_string;

char *
d_store_str_get(d_store *, size_t n);

#endif /* _DYN_STRING_ */
