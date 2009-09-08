#ifndef _DYN_VAR_
#define _DYN_VAR_

#include "dyn/store.h"

#define v_value(dv)     ((dv).dv_value)
#define v_size(dv)      ((dv).dv_size)

/**
 * v_type - get the variable type.
 */
#define v_type(dv)      ((dv).dv_type)

/**
 * i_integer - Fetch the integer value from a ds_array_item.
 */
#define v_integer(dv)  (v_value(dv).v_int)

/**
 * i_integer - Fetch the decimal value from a ds_array_item.
 */
#define v_decimal(dv)  (v_value(dv).v_decimal)

/**
 * i_integer - Fetch the string value from a ds_array_item.
 */
#define v_string(dv)   ((char *)(v_value(dv).v_pointer))

enum dtype
{
  Integer,
  String,
  Decimal
};

typedef struct dvar_s
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
} dvar;

#define v_init(ds, dv, type, n) \
do \
  { \
  switch(type) \
  { \
    case String: v_value(dv).v_pointer = ds_get(ds, n + 1); break; \
    /** other types need no allocation **/ \
    case Integer: v_size(dv) = sizeof(int); break; \
    case Decimal: v_size(dv) = sizeof(double); break; \
    default: break; \
  } \
  \
  v_type(dv) = type; } \
while(0)

#endif /* _DYN_VAR_ */
