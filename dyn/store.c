#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dyn/store.h"

#define current_pointer(ds) (ds)->d_pointers[(ds)->d_ppos]

// internal functions

/**
 * The backbone of ds_get.
 */
void *_ds_get(dstore *, size_t);

/**
 * Makes sure that it only uses the top memory pointer and expands it when necessary.
 * Used for streams.
 */
void *_ds_put_more(dstore *ds, const void *d, size_t n);

/**
 * Initiates the current pointer and prepares it for memory writing.
 * Expands the list of pointers when necessary.
 */
void _ds_init_current_pointer(dstore *ds);

void
ds_init(ds)
  dstore *ds;
{
  ds->d_state     = DS_NORMAL;
  ds->d_palloc    = D_POINTERS_INITIAL;
  ds->d_pointers  = malloc(ds->d_palloc * sizeof(D_MEMORY_TYPE *));
  
  // set the initial pointer.
  ds->d_ppos      = 0;
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
  assert(ds->d_state == DS_NORMAL);
  
  // reset pointer if it's not a block of zero
  if (ds->d_pos != 0)
  {
    // use the next pointer.
    ++ds->d_ppos;
    _ds_init_current_pointer(ds);
  }
  
  dss->ds = ds;
  dss->ss_pointer = NULL;
  dss->ss_pos = 0;
  dss->ss_closed = 0;
  
  dss->ds->d_state = DS_STREAM;
  return dss;
}

void
ds_stream_write(dss, c, n)
  dstream     *dss;
  const char  *c;
  size_t      n;
{
  assert(dss->ds->d_state == DS_STREAM);
  assert(dss->ss_closed == 0);
  
  dss->ss_pointer = _ds_put_more(dss->ds, c, n);
  dss->ss_pos += n;
}

char *
ds_stream_close(dss)
  dstream *dss;
{
  assert(dss->ds->d_state == DS_STREAM);
  assert(dss->ss_closed == 0);
  
  dss->ss_closed = 1;
  
  dss->ss_pointer = _ds_put_more(dss->ds, "\0", 1);
  dss->ds->d_state = DS_NORMAL;
  dss->ds->d_pos += dss->ss_pos;
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
        assert(ds->d_alloc <= D_SIZE_MAX);
        current_pointer(ds) = realloc(current_pointer(ds), ds->d_alloc * sizeof(D_MEMORY_TYPE));
        assert(current_pointer(ds) != NULL);
      }
      else
      {
        // use the next pointer.
        ++ds->d_ppos;
        _ds_init_current_pointer(ds);
      }
    }
  
  build     = ((D_MEMORY_TYPE *)current_pointer(ds) + ds->d_pos);
  ds->d_pos += n;
  
  bzero(build, n);
  return build;
}

void *
_ds_put_more(ds, d, n)
  dstore *ds;
  const void *d;
  size_t n;
{
  assert(ds->d_pointers != NULL);
  
  while (ds->d_pos + n >= ds->d_alloc)
    {
      ds->d_alloc *= D_REALLOC_FACTOR;
      assert(ds->d_alloc <= D_SIZE_MAX);
      current_pointer(ds) = realloc(current_pointer(ds), ds->d_alloc * sizeof(D_MEMORY_TYPE));
      assert(current_pointer(ds) != NULL);
    }
  
  memcpy(((D_MEMORY_TYPE *)current_pointer(ds) + ds->d_pos), d, n);
  ds->d_pos += n;
  
  return current_pointer(ds);
}

void
_ds_init_current_pointer(ds)
  dstore *ds;
{
  assert(ds->d_state == DS_NORMAL);
  
  if (ds->d_ppos >= ds->d_palloc)
    {
      ds->d_palloc *= D_REALLOC_FACTOR;
      assert(ds->d_palloc <= D_POINTERS_MAX);
      ds->d_pointers = realloc(ds->d_pointers, ds->d_palloc * sizeof(D_MEMORY_TYPE *));
    }
  
  ds->d_pos           = 0;
  ds->d_alloc         = D_SIZE_INITIAL;
  
  current_pointer(ds) = malloc(ds->d_alloc * sizeof(D_MEMORY_TYPE));
}

