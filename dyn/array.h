#ifndef _DYN_ARRAY_
#define _DYN_ARRAY_

#include <sys/types.h>

#include "dyn/store.h"
#include "dyn/var.h"

#define i_value(dai) ((dai)->dai_value)
#define i_next(dai) ((dai)->dai_next)
#define i_prev(dai) ((dai)->dai_prev)
#define i_type(dai) ((dai)->dai_type)

#define for_each_in(da, current) \
  for ((current) = (da)->da_first; (current) != NULL; (current) = i_next(current))

#define i_integer(dai)  (*((int *)i_value(dai)))
#define i_string(dai)   ((char *)i_value(dai))

#define dsa_size(dsa) ((dsa)->da_size)

typedef struct darray_s
{
  /* pointer to the first item in the array */
  struct darray_item_s  *da_first;
  /* pointer to the last item in the array */
  struct darray_item_s  *da_last;
  /* the actual array size */
  int                   da_size;
  /* the storage area for the string */
  dstore                *ds;
}
darray;

typedef struct darray_item_s
{
  enum dtype            dai_type;
  void                  *dai_value;
  size_t                dai_size;
  struct darray_item_s  *dai_next;
  struct darray_item_s  *dai_prev;
} darray_item;

void
dsa_init(dstore *, darray *);

darray_item *
dsa_append(darray *, void *, enum dtype, size_t);

darray_item *
dsa_pop(darray *);

#endif /* _DYN_ARRAY_ */
