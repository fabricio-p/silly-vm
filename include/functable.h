#ifndef FUNCTABLE_H
#define FUNCTABLE_H
#include <malloc.h>
#include <string.h>
#include <xxhash.h>
#include "silly.h"
#include "macros.h"

struct functable_hdr {
  Usize cap;
  Usize used;
  SFunc *data[1];
};

__inline__
SFuncTable SFuncTable_new(Usize const count)
{
  Usize const cap = count * 3 / 2;
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
__inline__
Ssize SFuncTable_insert(SFuncTable table, SFunc const *const func)
{
  Ssize status = FUNCTABLE_INSERT_OK;
  struct functable_hdr *const header = functable_header(table);
  const XXH32_hash_t hash = XXH32(func->name->str, func->name->len, 0);
  Usize const index = hash % header->cap;
  SFunc **slot = &(header->data[index]);
  SFunc **end = &(header->data[header->cap]);
  for (; slot < end; ++slot)
  {
    if (*slot == NULL)
    {
      *slot = (SFunc *)func;
      ++(header->used);
      break;
    } else
    {
      PStr const *const slot_name = (*slot)->name;
      PStr const *const func_name = func->name;
      if (slot_name->len == func_name->len &&
          memcmp(slot_name->str, func_name->str, slot_name->len) == 0)
      {
        status = FUNCTABLE_INSERT_DUP;
        break;
      }
    }
  }
  if (slot == end)
  {
    status = FUNCTABLE_INSERT_ERR;
  }
  return status;
}
__inline__
SFunc *SFuncTable_get(SFuncTable table, CStr name, Usize len)
{
  if (len == 0)
  {
    len = strlen(name);
  }
  struct functable_hdr *const header = functable_header(table);
  SFunc const *func = NULL;
  if (header->used <= 16)
  {
    for (SFunc *const *slot = &(header->data[0]),
               **end = &(header->data[header->cap]);
         slot < end;
         ++slot)
    {
      if (*slot != NULL && (*slot)->name->len == len &&
          memcmp((*slot)->name->str, name, len) == 0)
      {
        func = *slot;
        break;
      }
    }
  } else
  {
    XXH32_hash_t hash = XXH32(name, len, 0);
    Usize index = hash % header->cap;
    for (SFunc *const *slot = &(header->data[index]),
               *const *const end = &(header->data[header->cap]);
         slot < end;
         ++slot)
    {
      if (*slot == NULL)
      {
        break;
      } else if ((*slot)->name->len == len &&
                 memcmp((*slot)->name->str, name, len) == 0)
      {
        func = *slot;
      }
    }
  }
  return (SFunc *)func;
}

#define SFuncTable_cleanup(table)                                 \
  (free((void *)(table) - offsetof(struct functable_hdr, data)))

#endif /* FUNCTABLE_H */
