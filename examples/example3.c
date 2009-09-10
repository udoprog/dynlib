#include <stdio.h>

#include "dyn/store.h"
#include "dyn/var.h"

int main()
{
  dstore ds;
  dvar dv;
  
  ds_init(&ds);
  ds_var_init(&ds, &dv, String, 120);
  
  v_string_set(dv, "this is test");
  
  printf("Init value = %s\n", v_string(dv));
}
