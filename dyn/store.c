#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dyn/store.h"

#define current_pointer(ds) (ds)->d_pointers[(ds)->d_ppos]

// internal functions
void * _ds_get(dstore *, size_t);

void
_ds_init_current_pointer(ds)
  dstore *ds;
{
  assert(ds->d_state == DS_NORMAL);
  ds->d_pos           = 0;
  ds->d_alloc         = D_INITIAL_SIZE;
  
  current_pointer(ds) = malloc(ds->d_alloc * sizeof(int8_t));
}

void
ds_init(ds)
  dstore *ds;
{
  ds->d_state     = DS_NORMAL;
  ds->d_palloc    = D_INITIAL_POINTERS;
  ds->d_ppos      = 0;
  ds->d_pointers  = malloc(ds->d_palloc * sizeof(int8_t *));
  
  _ds_init_current_pointer(ds);
}

void
ds_free(ds)
  dstore *ds;
{
  int i = 0;
  
  assert(ds->d_state == DS_NORMAL);
  
  for (i = 0; i <= ds->d_ppos; i++)
    {
      free(ds->d_pointers[i]);
    }

  free(ds->d_pointers);
  
  ds->d_state = DS_FREE;
  ds->d_state     = 0x0;
  ds->d_palloc    = 0;
  ds->d_ppos      = 0;
  
  // pointer
  ds->d_pos       = 0;
  ds->d_alloc     = 0;
}

void *
ds_get(ds, n)
  dstore *ds;
  size_t n;
{
  assert(ds->d_state == DS_NORMAL);
  return _ds_get(ds, n);
}

size_t
ds_size(ds)
  dstore *ds;
{
  return ds->d_pos;
}

size_t
ds_stream_size(dss)
  dstream *dss;
{
  return dss->ss_pos;
}

dstream *
ds_stream_init(ds, dss)
  dstore *ds;
  dstream *dss;
{
  assert(0 == 1);
  dss->ds = ds;
  dss->ss_pointer = (char *)current_pointer(ds) + ds->d_pos;
  dss->ss_pos = 0;
  dss->ss_closed = 0;
  
  ds->d_state = DS_STREAM;
  return dss;
}

void
ds_stream_write(dss, c, n)
  dstream     *dss;
  const char  *c;
  size_t      n;
{
  assert(0 == 1);
  assert(dss->ds->d_state == DS_STREAM);
  assert(dss->ss_closed == 0);
  
  char *b = _ds_get(dss->ds, n);
  memcpy(b, c, n);
  dss->ss_pos += n;
}

char *
ds_stream_close(dss)
  dstream *dss;
{
  assert(0 == 1);
  assert(dss->ds->d_state == DS_STREAM);
  dss->ds->d_state = DS_NORMAL;
  assert(dss->ss_closed == 0);
  dss->ss_closed = 1;
  
  char *c = ds_get(dss->ds, 1);
  *c = '\0';
  return dss->ss_pointer;
}

// internal
void *
_ds_get(ds, n)
  dstore *ds;
  size_t n;
{
  void *build = NULL;
  
  assert(ds->d_pointers != NULL);
  
  while (ds->d_pos + n >= ds->d_alloc)
    {
      if (ds->d_pos == 0)
      {
        ds->d_alloc *= D_REALLOC_FACTOR;
        assert(ds->d_alloc <= D_MAX_SIZE);
        current_pointer(ds) = realloc(current_pointer(ds), ds->d_alloc * sizeof(int8_t));
        assert(current_pointer(ds) != NULL);
      }
      else
      {
        ++ds->d_ppos;
        
        if (ds->d_ppos >= ds->d_palloc)
          {
            ds->d_palloc *= D_REALLOC_FACTOR;
            ds->d_pointers = realloc(ds->d_pointers, ds->d_palloc * sizeof(int8_t *));
          }
        
        _ds_init_current_pointer(ds);
      }
    }
  
  build     = ((int8_t *)current_pointer(ds) + ds->d_pos);
  ds->d_pos += n;
  
  bzero(build, n);
  return build;
}
