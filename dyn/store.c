#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dyn/store.h"

// internal functions
void * _ds_get(dstore *, size_t);

void
ds_init(ds)
dstore *ds;
  {
    ds->d_alloc   = D_INITIAL_SIZE;
    ds->d_pointer = malloc(ds->d_alloc);
    ds->d_pos     = 0;
    ds->d_state   = DS_NORMAL;
  }

void
ds_free(ds)
dstore *ds;
  {
    assert(ds->d_state == DS_NORMAL);
    assert(ds->d_pointer != NULL);
    free(ds->d_pointer);
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
    dss->ds = ds;
    dss->ss_pointer = (char *)ds->d_pointer + ds->d_pos;
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
    int8_t *build = NULL;
    
    assert(ds->d_pointer != NULL);
    
    if (ds->d_pos + n >= ds->d_alloc)
      {
        ds->d_alloc    *= D_REALLOC_FACTOR;
        assert(ds->d_alloc <= D_MAX_SIZE);
        ds->d_pointer   = realloc(ds->d_pointer, ds->d_alloc);
        assert(ds->d_pointer != NULL);
      }
    
    build      = (int8_t *)ds->d_pointer + ds->d_pos;
    ds->d_pos += n;
    
    bzero(build, n);
    return build;
  }
