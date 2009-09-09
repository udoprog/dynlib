#include "dyn/var.h"

void
ds_var_init(ds, dv, dt, n)
  dstore      *ds;
  dvar        *dv;
  enum dtype  dt;
  size_t      n;
{
  switch(dt)
  {
    case String: ++n, v_pointer(*dv) = ds_get(ds, n); break;
    /** other types need no allocation **/
    case Integer: n = sizeof(int);    break;
    case Decimal: n = sizeof(double); break;
    default: break;
  }
  
  v_size(*dv) = n;
  v_type(*dv) = dt;
}
