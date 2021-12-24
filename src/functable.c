#include <malloc.h>
#include <string.h>
#include <xxhash.h>
#include "functable.h"

static SStatus expand(SFuncTable *table_p)
{
  SStatus status = SILLY_E_OK;
  SFuncTable table = *table_p;
  struct functable_hdr const *const header = functable_header(table);
  SFuncTable new_table = SFuncTable_new(header->cap * 2);
  if (new_table == NULL)
  {
    status = SILLY_E_OOM;
    goto catch;
  }
  for (SFunc *const *slot = &(table[0]),
             *const *const end = &(table[header->cap]),
             *current = table[0];
       slot < end; current = *(++slot))
  {
    if (current != NULL)
    {
      // Should never fail (probably)
      SFuncTable_insert(&new_table, current);
    }
  }
  SFuncTable_cleanup(table);
  *table_p = new_table;
catch:
  return status;
}

Ssize SFuncTable_insert(SFuncTable *table_p, SFunc const *const func)
{
  Ssize status = FUNCTABLE_INSERT_OK;
  struct functable_hdr *const header = functable_header(*table_p);
  const XXH32_hash_t hash = XXH32(func->name.str, func->name.len, 0);
  Usize const index = hash % header->cap;
  if (header->used * 2 / 3 > header->cap)
  {
    if ((status = expand(table_p)) != SILLY_E_OK)
    {
      goto catch;
    }
  }
  SFuncTable table = *table_p;
  SFunc **slot = &(table[index]);
  SFunc **end = &(table[header->cap]);
  for (; slot < end; ++slot)
  {
    if (*slot == NULL)
    {
      *slot = (SFunc *)func;
      ++(header->used);
      break;
    } else
    {
      SFunc *current = *slot;
      if (current->name.len == func->name.len &&
          memcmp(current->name.str, func->name.str,
                 current->name.len) == 0)
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
catch:
  return status;
}


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
    for (SFunc *const *slot = &(table[0]),
               **end = &(table[header->cap]);
         slot < end;
         ++slot)
    {
      if (*slot != NULL && (*slot)->name.len == len &&
          memcmp((*slot)->name.str, name, len) == 0)
      {
        func = *slot;
        break;
      }
    }
  } else
  {
    XXH32_hash_t hash = XXH32(name, len, 0);
    Usize index = hash % header->cap;
    for (SFunc *const *slot = &(table[index]),
               *const *const end = &(table[header->cap]);
         slot < end;
         ++slot)
    {
      if (*slot == NULL)
      {
        break;
      } else if ((*slot)->name.len == len &&
                 memcmp((*slot)->name.str, name, len) == 0)
      {
        func = *slot;
      }
    }
  }
  return (SFunc *)func;
}
