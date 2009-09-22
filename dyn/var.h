#ifndef _DYN_VAR_
#define _DYN_VAR_

#include <string.h>

#include "dyn/store.h"

/**
 * v_value - get the value of the variable.
 */
#define v_value(dv)     ((dv).dv_value)

/**
 * v_size - determine the size of the variable.
 */
#define v_size(dv)      ((dv).dv_size)

/**
 * v_type - get the variable type.
 */
#define v_type(dv)      ((dv).dv_type)

/**
 * v_integer - Fetch the integer value from a d_store_array_item.
 */
#define v_integer(dv)  (v_value(dv).v_int)

/**
 * v_decimal - Fetch the decimal value from a d_store_array_item.
 */
#define v_decimal(dv)  (v_value(dv).v_decimal)

/**
 * v_string - Fetch the string value from a d_store_array_item.
 */
#define v_string(dv)   ((char *)(v_value(dv).v_pointer))

/**
 * v_pointer - Fetch the pointer part of the d_var.
 */
#define v_pointer(dv)   ((v_value(dv).v_pointer))

/**
 * v_string_set - Set value of a string.
 */
#define v_string_set(dv, str) do { \
  memset(v_pointer(dv), 0x0, v_size(dv)); \
  strncpy(v_pointer(dv), str, v_size(dv)); \
} while(0)

/**
 * v_string_size - Get the size of a string variable.
 */
#define v_string_size(dv) (v_size(dv) - 1)

enum dtype
{
  Integer,
  String,
  Decimal
};

typedef struct d_var_s
{
  enum dtype              dv_type;
  union {
    /**
     * v_pointer - Varying length storage type.
     */
    void    *v_pointer;
    /**
     * v_int - Fixed length storage type.
     */
    int     v_int;
    /**
     * v_long - Fixed length long storage.
     */
    long    v_long;
    /**
     * v_decimal - Fixed length decimal storage.
     */
    double  v_decimal;
  } dv_value;
  
  /**
   * dv_size - Size of storage.
   */
  size_t                  dv_size;
} d_var;

void
d_var_init(d_store *ds, d_var *dv, enum dtype dt, size_t n);

#endif /* _DYN_VAR_ */
