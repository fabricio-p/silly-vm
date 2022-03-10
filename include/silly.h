#ifndef SILLY_H
#define SILLY_H
#include <stdlib.h>
#include <stdint.h>
#include "config.h"

// Typedefs of basic primitive types and structs
typedef unsigned char      U8;
typedef signed   char      S8;
typedef unsigned short     U16;
typedef signed   short     S16;
typedef unsigned long      U32;
typedef signed   long      S32;
typedef unsigned long long U64;
typedef signed   long long S64;
typedef          int       Int;
typedef unsigned int       Uint;
typedef signed   int       Sint;

typedef float      F32;
typedef double     F64;
typedef U32        Usize;
typedef S32        Ssize;
typedef char       *Str;
typedef char const *CStr;
typedef void       *voidptr;
typedef U8         SValueKind;
typedef U8         Bool;
typedef U8         *ByteVec;

#ifdef DEBUG // so gdb shows us this instead of just integers during debug
#define E(e) SILLY_E_##e
typedef enum {
  E(OK) = 0,
  E(HALT),
  E(OOM),
  E(SO),
  E(CDE),
  E(CRASH),
  E(PARSE_INVALID_TYPE_SIZE)
} SStatus;
#undef E
#else
#define SILLY_E_OK    0
#define SILLY_E_HALT  1
#define SILLY_E_OOM   2 // (O)ut (O)f (M)emory
#define SILLY_E_SO    3 // (S)tack (O)verflow
#define SILLY_E_CDE   4 // (C)all (D)epth (E)xceeded
#define SILLY_E_CRASH 5
// Errors that occur only during bytecode parsing
#define SILLY_E_PARSE_INVALID_TYPE_SIZE 6

typedef Ssize      SStatus;
#endif

typedef struct SStackFrame SStackFrame;
typedef struct SCallFrame  SCallFrame;
typedef struct SEnv        SEnv;
typedef struct SProcess    SProcess;

// Internal VM structs
typedef struct SType {
  U16 param_count;
  U16 result_count;
  U8  types[1];
} SType;

typedef struct SFunc {
  SType const *type;
  U8    const *code;
  U8    const *code_end;
  // PStr  const *name;
  struct {
    Usize len;
    CStr  str;
  }           name;
  /* struct {
    Usize max : 15;
    Usize is_reported : 1;
  } stack_usage; */
  void  (*native)(SEnv *, SProcess *);
} SFunc;
typedef SFunc **SFuncTable;

typedef U32 SPid;

#ifdef DEBUG
typedef enum SMessageKind {
#define K(k) SILLY_MESSAGE_##k
  K(UNINITIALISED) = -1,
  K(SIGNAL),
  K(DATA)
#undef K
} SMessageKind;
#else
typedef signed char SMessageKind;
#define SILLY_MESSAGE_UNINITIALISED -1
#define SILLY_MESSAGE_SIGNAL        0
#define SILLY_MESSAGE_DATA          1
#endif

typedef struct SMessage {
  U64          tag;
  union {
    Sint    signal;
    ByteVec data;
  };
  SPid         sender;
#if SILLY_CPU_32BIT != 0
#define PADDING sizeof(U64) - sizeof(SMessageKind)
#else
#define PADDING sizeof(U32) - sizeof(SMessageKind)
#endif
  U8           __padd[PADDING];
#undef PADDING
  SMessageKind kind;
}__attribute__((packed)) SMessage;

// A stack value
typedef struct SValue {
  union {
    U32      u32;
    S32      s32;
    F32      f32;
    U64      u64;
    S64      s64;
    F64      f64;
  };
  SValueKind kind;
  U8 __padding[16 - sizeof(U64) - sizeof(SValueKind)];
}__attribute__((packed)) SValue;

typedef struct SStack SStack;

struct SCallFrame {
  // (s)tack (l)ocals, the pointer to where the locals start
  SValue      *sl;
  // (s)tack (b)ottom, the pointer to where after the locals
  SValue      *sb;
  // (s)tack (t)op, the pointer at the slot right after the last used one
  SValue      *st;
  // the pointer to the function that called
  SFunc       *function;
  SStack      *stack;
  Int         frame_index;
  // will tell at which opcode failure occurred if it does
  U8 const    *ip;
  // the size of the stack in SValues it can fit
  // U32         size;
#define ALIGN(n) (((n) + 7) & ~7)
#define DATA_SIZE (sizeof(uintptr_t) * 7)
  U8          __padd[ALIGN(DATA_SIZE) - DATA_SIZE];
#undef ALIGN
#undef DATA_SIZE
}__attribute__((packed));

#define SILLY_STACK_SIZE 0x800
#define SILLY_CALL_DEPTH 0x100
struct SStack {
  SValue     data[SILLY_STACK_SIZE];
  SCallFrame frames[SILLY_CALL_DEPTH];
  SCallFrame *current_frame;
#define ALIGN(n) (((n) + 7) & ~7)
#define DATA_SIZE                         \
  (sizeof(voidptr) +                      \
   sizeof(SValue) * SILLY_STACK_SIZE +    \
   sizeof(SCallFrame) * SILLY_CALL_DEPTH)
  U8          __padd[ALIGN(DATA_SIZE) - DATA_SIZE];
#undef ALIGN
#undef DATA_SIZE
}__attribute__((packed));

typedef struct SProcInfo {
  U32 init_func_idx;
  U32 listener_func_idx;
  U32 finish_func_idx;
  struct {
    U16 min;
    U16 max;
  }   memory;
} SProcInfo;

typedef struct SModule {
  CStr       path;
  struct {
    U8  *buffer;
    U32 type_sec_size;
    U32 func_sec_size;
    U32 data_sec_size;
    U32 code_sec_size;
    U32 proc_sec_size;
  }          raw;
  struct {
    voidptr types_buffer;
    voidptr bytecode_buffer;
    U8      *data_buffer;
  }          loaded;
  SType      **types;
  U8         **data; // like, from the data section. idk why I need this
  SProcInfo  *processes;
  SFunc      *functions;
  SFuncTable func_table;
  SFuncTable exports;
} SModule;

/*
 * typedef struct SQueuedProcess SQueuedProcess;
 * typedef struct SQueue         SQueue;
 */
typedef struct AllocNode      AllocNode;

struct SEnv {
  // SQueue    proc_queue;
  union {
    SProcess *all_processes;
    SProcess *main_process;
  };
  SModule   *module_pool;
  SFunc     *func_pool;
  AllocNode *type_pool_chain;
};

#endif /* SILLY_H */
