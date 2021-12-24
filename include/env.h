#ifndef ENV_H
#define ENV_H
#include "silly.h"

struct AllocNode {
  Uint      size;
  AllocNode *next;
  AllocNode *prev;
  U8        __padding[sizeof(Uint) == 8 ? 0 : 4];
  U8        data[1];
};

SStatus AllocChain_alloc(AllocNode **, Usize, voidptr *);
void    AllocChain_free(AllocNode **);

#endif /* ENV_H */
