#include <sys/types.h>

#include <stdlib.h>
#include <string.h>

#include "dyn/array.h"
#include "dyn/store.h"
#include "dyn/var.h"

void
d_store_array_init(ds, dsa)
  d_store *ds;
  d_store_array *dsa;
{
  dsa->da_first = NULL;
  dsa->da_last  = NULL;
  dsa->da_size  = 0;
  dsa->ds       = ds;
}

d_store_array_item *
d_store_array_append(dsa, var, dt, n)
  d_store_array      *dsa;
  void          *var;
  enum dtype    dt;
  size_t        n;
{
  d_store_array_item *prev    = NULL;
  d_store_array_item *current = dsa->da_first;

  while (current != NULL)
    {
      prev = current;
      current = current->dai_next;
    }
  
  if (prev == NULL)
    {
      dsa->da_first = d_store_get(dsa->ds, sizeof(d_store_array_item));
      current = dsa->da_first;
    }
  else
    {
      prev->dai_next = d_store_get(dsa->ds, sizeof(d_store_array_item));
      current = prev->dai_next;
    }
  
  dsa->da_last = current;
  
  current->dai_prev = prev;
  current->dai_next = NULL;
  
  d_var_init(dsa->ds, &i_value(current), dt, n);
  
  /* store the correct type of value. */
  switch (dt)
    {
    case Integer: i_integer(current) = *((int *)var);           break;
    case Decimal: i_decimal(current) = *((double *)var);        break;
    case String: v_string_set(i_value(current), ((char *)var)); break;
    default: break;
    }
  
  ++dsa->da_size;
  return current;
}

d_store_array_item *
d_store_array_pop(dsa)
  d_store_array        *dsa;
{
  d_store_array_item *last = dsa->da_last;
  
  if (last == NULL) 
    {
      return NULL;
    }
  
  // are we at the beginning of the array?
  if (last->dai_prev == NULL)
    {
      dsa->da_last = NULL;
      dsa->da_first = NULL;
    }
  // we are not at the beginning of the array.
  else
    {
      dsa->da_last = last->dai_prev;
      last->dai_prev->dai_next = NULL;
    }
  
  --dsa->da_size;
  return last;
}
