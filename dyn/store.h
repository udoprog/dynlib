#ifndef _DYN_STORE_
#define _DYN_STORE_

#define D_INITIAL_SIZE    1024
#define D_MAX_SIZE        1048576
#define D_REALLOC_FACTOR  2

enum state_t {
  DS_NORMAL,
  DS_STREAM
};

#include <sys/types.h>

typedef struct dstore_s
{
  /* allocation position */
  size_t        d_pos;
  /* pointer to memory */
  void          *d_pointer;
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
dss_size(dstream *);

/**
 * dss_init - Initiate a dynamic storage stream.
 * @1 - the dynamic storage to initiate the stream against.
 * @2 - pointer to a stream to initiate.
 */
dstream *
dss_init(dstore *, dstream *);

/**
 * dss_write - Write data to a dynamic storage stream.
 * @1 - the dynamic storage stream which governs the data.
 * @2 - the actual string to write.
 * @3 - the length of the string to write.
 */
void
dss_write(dstream *, const char *, size_t);

/**
 * dss_close - Close a dynamic storage stream.
 *
 */
char *
dss_close(dstream *);

#endif /* _DYN_STORE_ */
