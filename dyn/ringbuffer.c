/*
 * Implements a fix sized database in memory for storing statistical information.
 * Works like a ring buffer, 
 */

#include <stdlib.h>
#include <string.h>

#include "dyn/store.h"
#include "dyn/ringbuffer.h"

void
d_ringbuffer_init(ds, drb, n, rs)
  d_store *ds;
  d_ringbuffer *drb;
  unsigned int n;
  size_t rs;
{
  drb->d_ringbuffer_ds =          ds;
  drb->d_ringbuffer_memory =      d_store_get(ds, n * rs);
  
  bzero(drb->d_ringbuffer_memory, n * rs);
  
  drb->d_ringbuffer_size =        n;
  drb->d_ringbuffer_record_size = rs;
  drb->d_ringbuffer_pos =         0;
  drb->d_ringbuffer_full =        0;
  return;
}

void
d_ringbuffer_insert(db, record)
  d_ringbuffer     *db;
  void            *record;
{
  /* case 1 - we are not at the end of the buffer */
  if (db->d_ringbuffer_pos >= db->d_ringbuffer_size)
  {
    db->d_ringbuffer_pos    = 0;
    db->d_ringbuffer_full   = 1;
    goto end;
  }
  
end:
  memcpy(db->d_ringbuffer_memory + db->d_ringbuffer_pos * db->d_ringbuffer_record_size, record, db->d_ringbuffer_record_size);
  ++(db->d_ringbuffer_pos);
  return;
}

void *
d_ringbuffer_get(db, n)
  d_ringbuffer *db;
  unsigned int n;
{
  if (db->d_ringbuffer_full)
    return db->d_ringbuffer_memory + (((db->d_ringbuffer_pos + n) * db->d_ringbuffer_record_size) % (db->d_ringbuffer_size * db->d_ringbuffer_record_size));
  else
    return db->d_ringbuffer_memory + ((n * db->d_ringbuffer_record_size));
}
