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

struct SProcess {
  SStack    stack;
  U8        *memory;
#ifdef SILLY_PROCESS_TRACING
  SModule   *module;
  SProcInfo *info;
#endif
  SFunc     *init_func; // fn(SMessage <the init argument(s)>): void
  SFunc     *finish_func; // fn(void): void
  SFunc     *listener; // fn(SMessage): SMessage
  SProcess  *children;
  SProcess  *parent;
  SMessage  message;
  SPid      pid;
  S32       exit_code;
  U16       memory_size;
};

#define CVECTOR_POINTERMODE
// #define CVECTOR_NO_TYPEDEF
#include <c-vector/lib.h>

CVECTOR_WITH_NAME(SProcess,   SProcesses);
CVECTOR_WITH_NAME(SProcess *, SChildProcesses);

/* struct SQueuedProcess {
  SPid proc_pid;
  // the index of the queued-process that will be processed before this one
  // indices instead of pointers because dead processes will be removed
  // from the process array?
  U32  prev_qpi;
  U32  next_qpi;
};
struct SQueue {
  SQueuedProcess *processes;
  U32            first_idx;
  U32            last_idx;
}; */

SStatus AllocChain_alloc(AllocNode **, Usize, voidptr *);
void    AllocChain_free(AllocNode **);

void    SProcess_create(SEnv *, SProcess *, SProcess *, SProcInfo *,
                        SModule *);
SStatus SProcess_spawn(SEnv *, SProcess *, SMessage const *);

#endif /* ENV_H */
