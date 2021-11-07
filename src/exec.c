#include "silly.h"
#include "macros.h"
#include "exec.h"
#include "code.h"
#include "internal.h"
#define HANDLE_INSTR(instr) case SILLY_INSTR_##instr:
#define HANDLE_CONST_INSTR(type, field)   \
HANDLE_INSTR(CONST_##type) {              \
  type value = READ_##type(ip);           \
  ip += sizeof(type);                     \
  push(value, type, field);               \
  break;                                  \
}

#define push(val, type, field) {          \
  st->field = (val);                      \
  st->kind = (SILLY_TYPE_##type);         \
  ++st;                                   \
  cache.field = (val);                    \
  cache.kind = (SILLY_TYPE_##type);       \
}

SStatus SFunc_exec(SEnv *env, SCallFrame *frame)
{
  SStatus status = SILLY_S_OK;
  U8 const *ip = frame->function->code;
  U8 const *const end = frame->function->code_end;
  SValue cache = { .kind = 0, .u64 = 0 };
  // SValue *sl = frame->sl;
  // SValue *sb = frame->sb;
  SValue *st = frame->st;
  for (; ip < end;)
  {
    print_instruction(*ip);
    switch (*(ip++))
    {
      HANDLE_CONST_INSTR(U32, u32);
      HANDLE_CONST_INSTR(S32, s32);
      HANDLE_CONST_INSTR(U64, u64);
      HANDLE_CONST_INSTR(S64, s64);
      HANDLE_CONST_INSTR(F32, f32);
      HANDLE_CONST_INSTR(F64, f64);
      HANDLE_INSTR(ADD_U32)
      {
        cache.u32 = (st[-2].u32 += cache.u32);
        --st;
        break;
      }
    }
  }
  frame->st = st;
  frame->ip = ip;
  frame->cache = cache;
  return status;
}
