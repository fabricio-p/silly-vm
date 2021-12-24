#ifndef SILLY_H
#define SILLY_H
#include <stdlib.h>
#include <stdint.h>
// #include <c-bump-alloc/lib.h>

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

/* typedef struct PStr {
  U32 len;
  U8  str[1];
} PStr; */

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
  // void  (*native)(SCallFrame *);
} SFunc;
typedef SFunc **SFuncTable;

// A stack value
typedef struct SValue {
  union {
    U32 u32;
    S32 s32;
    F32 f32;
    U64 u64;
    S64 s64;
    F64 f64;
  };
  SValueKind kind;
  U8 __padding[16 - sizeof(U64) - sizeof(SValueKind)];
} SValue;

typedef struct SStackFrame SStackFrame;
typedef struct SCallFrame  SCallFrame;
typedef struct SEnv        SEnv;

typedef struct SStack {
  SValue     *data;
  U32        size;
  U32        max_size;
  SCallFrame *frames;
  U32        call_depth;
} SStack;

typedef struct SMemory {
  U32 page_count;
  U32 max_pages;
  U8  data;
} SMemory;

struct SCallFrame {
  // the value at the stack top
  SValue      cache;
  // (s)tack (l)ocals, the pointer to where the locals start
  SValue      *sl;
  // (s)tack (b)ottom, the pointer to where the locals end and operational
  // stack where you push and pop starts
  SValue      *sb;
  // (s)tack (t)op, the pointer at the slot right after the last used one
  SValue      *st;
  // the pointer to the function that called
  SFunc       *function;
  SStack      *stack;
  U32         frame_index;
  // will tell at which opcode failure occurred if it occurs
  U8 const    *ip;
  // the size of the stack in SValues it can fit
  U32         size;
};

typedef struct SModuleInfo SModuleInfo;

typedef struct SModule {
  CStr       path;
  struct {
    U8  *buffer;
    U32 type_sec_size;
    U32 func_sec_size;
    U32 data_sec_size;
    U32 code_sec_size;
    struct {
      U16 min;
      U16 max;
    }   mem_cfg;
  }          raw;
  struct {
    voidptr types_buffer;
    voidptr bytecode_buffer;
    U8      *data_buffer;
  }          loaded;
  SType      **types;
  U8         **data; // like, from the data section. idk why I need this
  SFuncTable func_table;
  SFuncTable exports;
} SModule;

typedef struct AllocNode AllocNode;

struct SEnv {
  SStack    stack;
  SMemory   memory;
  SModule   *module_pool;
  SFunc     *func_pool;
  AllocNode *type_pool_chain;
};

#endif /* SILLY_H */
