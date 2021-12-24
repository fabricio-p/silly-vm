#ifndef FUNCTABLE_H
#define FUNCTABLE_H
#include "silly.h"
#include "macros.h"

struct functable_hdr {
  Usize cap;
  Usize used;
  SFunc *data[1];
};

__inline__
SFuncTable SFuncTable_new(Usize const cap)
{
  Usize const size = ((cap * sizeof(SFunc *)) +
                      offsetof(struct functable_hdr, data));
  struct functable_hdr *header = malloc(size);
  SFuncTable table = NULL;
  if (header != NULL)
  {
    memset(header, '\0', size);
    header->cap = cap;
    header->used = 0;
    table = &(header->data[0]);
  }
  return table;
}
// Means that the insertion went as expected
#define FUNCTABLE_INSERT_OK  0
// Means that the key is duplicate, won't insert
#define FUNCTABLE_INSERT_DUP 1
// Means that it couldn't insert
#define FUNCTABLE_INSERT_ERR 3

#define functable_header(table)                               \
  ((struct functable_hdr *)                                   \
   ((void *)(table) - offsetof(struct functable_hdr, data)))
Ssize SFuncTable_insert(SFuncTable *, SFunc const *const);
SFunc *SFuncTable_get(SFuncTable, CStr, Usize);

#define SFuncTable_cleanup(table)                                 \
  (free((void *)(table) - offsetof(struct functable_hdr, data)))

#endif /* FUNCTABLE_H */
