#ifndef _DYN_ARRAY_
#define _DYN_ARRAY_

#include <sys/types.h>

#include "dyn/store.h"
#include "dyn/var.h"

#define i_value(dai) ((dai)->val)
#define i_next(dai) ((dai)->dai_next)
#define i_prev(dai) ((dai)->dai_prev)
#define i_type(dai) (v_type(i_value(dai)))
#define i_size(dai) (v_size(i_value(dai)))

/**
 * for_each_in - Used for iterating arrays.
 * @da - Pointer to the array to iterate.
 * @c - Pointer to a d_store_array_item which will work as iterator.
 */
#define for_each_in(da, c) \
  for ((c) = (da)->da_first; (c) != NULL; (c) = i_next(c))

#define d_store_array_size(dsa) ((dsa)->da_size)

/**
 * i_integer - Fetch the integer value from a d_store_array_item.
 */
#define i_integer(dai)  (v_integer(i_value(dai)))

/**
 * i_integer - Fetch the decimal value from a d_store_array_item.
 */
#define i_decimal(dai)  (v_decimal(i_value(dai)))

/**
 * i_integer - Fetch the string value from a d_store_array_item.
 */
#define i_string(dai)   (v_string(i_value(dai)))

typedef struct d_store_array_s
{
  /* pointer to the first item in the array */
  struct d_store_array_item_s  *da_first;
  /* pointer to the last item in the array */
  struct d_store_array_item_s  *da_last;
  /* the actual array size */
  int                   da_size;
  /* the storage area for the string */
  d_store                *ds;
}
d_store_array;

typedef struct d_store_array_item_s
{
  struct d_var_s           val;
  
  /**
   * dai_next - Pointer to next array item.
   */
  struct d_store_array_item_s  *dai_next;

  /**
   * dai_prev - Pointer to previous array item.
   */
  struct d_store_array_item_s  *dai_prev;
} d_store_array_item;

/**
 * d_store_array_init - Initiate array storage.
 * @ds - Pointer to d_store instance (must have been initiated).
 * @dsa - Pointer to darray.
 *
 * It is important that you initiate an array before using since it might 
 * otherwise be in an undefined state.
 *
 * All information stored in the array will be stored in the 'd_store' space 
 * instead, whereas necessary space will be allocated on demand.
 */
void
d_store_array_init(d_store *ds, d_store_array *dsa);

/**
 * d_store_array_append - Append a item to an array.
 * @dsa - Pointer to the array which will be appended.
 * @val - Pointer to the value which should be appended.
 * @dt - enum telling which type of value to append.
 * @n - size of value which is appended, this is automatically calculated for 
 * fixed width values.
 *
 * @return The value which is appended.
 */
d_store_array_item *
d_store_array_append(d_store_array *dsa, void *val, enum dtype dt, size_t n);

/**
 * d_store_array_pop - Pop a value from an array.
 * @dsa - Array to pop from.
 *
 * @return The value which is popped.
 */
d_store_array_item *
d_store_array_pop(d_store_array *dsa);

#endif /* _DYN_ARRAY_ */
