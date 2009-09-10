#ifndef _DYN_STORE_
#define _DYN_STORE_

#include <stdint.h>

#define D_SIZE_INITIAL      4096
#define D_SIZE_MAX          4294967296
#define D_POINTERS_INITIAL  64
#define D_POINTERS_MAX      268435456

#define D_REALLOC_FACTOR    2
/* this is the unit in which memory is allocated, therefore
 * sizeof(D_MEMORY_TYPE) is the smallest possible allocation */
#define D_MEMORY_TYPE       int8_t

enum state_t {
  /** normal mode **/
  DS_NORMAL,
  /** used when a store is used as a stream, to block normal operations, since 
      memory is stupid and continous **/
  DS_STREAM,
  /** used when it has been freed, should trigger asserts if you try to use it 
      after it has been freed **/
  DS_FREE
};

#include <sys/types.h>

typedef struct dstore_s
{
  /* allocation position */
  size_t        d_pos;
  /* allocation position */
  size_t        d_palloc;
  /* the current pointer */
  size_t        d_ppos;
  /* pointer to memory */
  D_MEMORY_TYPE **d_pointers;
  /* amount allocated */
  size_t        d_alloc;
  /* state, if open as stream for example. */
  enum state_t  d_state;
}
dstore;

typedef struct dstream_s
{
  size_t              ss_pos;
  char                *ss_pointer;
  dstore              *ds;
  short unsigned int  ss_closed;
}
dstream;

void
ds_init(dstore *);

void
ds_free(dstore *);

void *
ds_get(dstore *, size_t);

size_t
ds_size(dstore *);

size_t
ds_stream_size(dstream *);

/**
 * ds_stream_init - Initiate a dynamic storage stream.
 * @1 - the dynamic storage to initiate the stream against.
 * @2 - pointer to a stream to initiate.
 */
dstream *
ds_stream_init(dstore *, dstream *);

/**
 * ds_stream_write - Write data to a dynamic storage stream.
 * @1 - the dynamic storage stream which governs the data.
 * @2 - the actual string to write.
 * @3 - the length of the string to write.
 */
void
ds_stream_write(dstream *, const char *, size_t);

/**
 * ds_stream_close - Close a dynamic storage stream.
 *
 */
char *
ds_stream_close(dstream *);

#endif /* _DYN_STORE_ */
