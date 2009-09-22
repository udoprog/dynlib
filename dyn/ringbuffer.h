#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "dyn/store.h"

typedef struct d_ringbuffer_st
{
  d_store        *d_ringbuffer_ds;
  unsigned int  d_ringbuffer_size;
  unsigned int  d_ringbuffer_pos;
  unsigned int  d_ringbuffer_full;
  size_t        d_ringbuffer_record_size;
  D_MEMORY_TYPE *d_ringbuffer_memory;
} d_ringbuffer;

void
d_ringbuffer_init(d_store *, d_ringbuffer *, unsigned int, size_t);

void
d_ringbuffer_insert(d_ringbuffer *, void *);

void *
d_ringbuffer_get(d_ringbuffer *, unsigned int);

#endif /* _RINGBUFFER_H_ */
