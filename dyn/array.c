#include <sys/types.h>

#include <stdlib.h>
#include <string.h>

#include "dyn/array.h"
#include "dyn/store.h"

void
ds_array_init(ds, dsa)
  dstore *ds;
  ds_array *dsa;
{
  dsa->da_first = NULL;
  dsa->da_last  = NULL;
  dsa->da_size  = 0;
  dsa->ds       = ds;
}

ds_array_item *
ds_array_append(dsa, var, dt, n)
  ds_array      *dsa;
  void          *var;
  enum dtype    dt;
  size_t        n;
{
  ds_array_item *prev    = NULL;
  ds_array_item *current = dsa->da_first;

  switch (dt)
    {
    // integers have fixed size
    case Integer: n = sizeof(int);    break;
    case Decimal: n = sizeof(double); break;
    // strings are null terminated, therefore one extra character.
    case String:  ++n;                break;
    default:                          break;
    }
  
  while (current != NULL)
    {
      prev = current;
      current = current->dai_next;
    }
  
  if (prev == NULL)
    {
      dsa->da_first = ds_get(dsa->ds, sizeof(ds_array_item));
      current = dsa->da_first;
    }
  else
    {
      prev->dai_next = ds_get(dsa->ds, sizeof(ds_array_item));
      current = prev->dai_next;
    }
  
  dsa->da_last = current;
  
  current->dai_prev = prev;
  
  i_size(current) = n;
  i_type(current) = dt;
  current->dai_next = NULL;
  
  /* store the correct type of value. */
  switch (dt)
    {
    case Integer: i_integer(current) = *((int *)var);     break;
    case Decimal: i_decimal(current) = *((double *)var);  break;
    case String:              
      v_value(i_value(current)).v_pointer = ds_get(dsa->ds, n);
      memcpy(i_string(current), var, n);
      break;
    default:                        break;
    }
  
  ++dsa->da_size;
  return current;
}

ds_array_item *
ds_array_pop(dsa)
  ds_array        *dsa;
{
  ds_array_item *last = dsa->da_last;
  
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
