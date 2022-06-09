#include <c-ansi-sequences/graphics.h>
#include "silly.h"
#include "macros.h"
#include "exec.h"
#include "code.h"
#include "internal.h"
#include "util.h"
#define HANDLE_INSTR(instr) case SILLY_INSTR_##instr:
#define HANDLE_CONST_INSTR(type, field)   \
HANDLE_INSTR(CONST_##type) {              \
  type value = READ_##type(ip);           \
  ip += sizeof(type);                     \
  push(value, type, field);               \
  break;                                  \
}

#define push(val, type, field) {          \
  st->v.field = (val);                    \
  st->kind = (SILLY_TYPE_##type);         \
  ++st;                                   \
  cache.v.field = (val);                  \
  cache.kind = (SILLY_TYPE_##type);       \
}
#define HANDLE_BINOP_INSTR(type, field, op, opname)     \
HANDLE_INSTR(opname##_##type) {                         \
  --st;                                                 \
  cache.v.field = (st[-1].v.field op##= cache.v.field); \
  break;                                                \
}
#define HANDLE_UNOP_INSTR(type, field, opname, action)  \
HANDLE_INSTR(opname##_##type) {                         \
  cache.v.field = st[-1].v.field = (action);            \
}

#define HALT_E_CREATE(exit_code)                      \
  (((exit_code) << (sizeof(U8) * 3)) | SILLY_E_HALT)

SStatus SFunc_exec(SEnv *env, SCallFrame *frame)
{
  SStatus status = SILLY_E_OK;
  U8 const *ip = frame->function->code;
  U8 const *const end = frame->function->code_end;
  STaggedValue cache = {0};
  STaggedValue *sl = frame->sl;
  // STaggedValue *sb = frame->sb;
  STaggedValue *st = frame->st;
  for (; ip < end;)
  {
#ifdef ENV_DEV
#ifdef OUTCOLORS
#define IP_FMT(s) ANSISEQ_SETFG256(154) s ANSISEQ_GR_RESET
#else
#define IP_FMT(s) s
#endif /* OUTCOLORS */
    printf("[ip: "IP_FMT("0x%08x")"] ",
           ip - frame->function->code);
#endif
    print_instruction(*ip);
    switch (*(ip++))
    {
      HANDLE_INSTR(NOP)
      {
        break;
      }
      HANDLE_INSTR(HALT_VM)
      {
        S32 exit_code = cache.v.s32;
        --st;
        status = HALT_E_CREATE(exit_code);
        goto outside;
      }
      HANDLE_INSTR(DUPLICATE_ANY)
      {
        st[1].kind = cache.kind;
        st[1].v.u64  = cache.v.u64;
        ++st;
        break;
      }
      HANDLE_INSTR(DROP_ANY)
      {
        --st;
        cache.kind = st[0].kind;
        cache.v.u64  = st[0].v.u64;
        break;
      }
      HANDLE_INSTR(GET_LOCAL)
      {
        ++st;
        U16 index = READ_U16(ip);
        ip += sizeof(U16);
        STaggedValue *local = &(sl[index]);
        st[0].kind = cache.kind = local->kind;
        st[0].v.u64  = cache.v.u64  = local->v.u64;
        break;
      }
      HANDLE_INSTR(SET_LOCAL)
      {
        U16 index = READ_U16(ip);
        ip += sizeof(U16);
        STaggedValue *local = &(sl[index]);
        local->kind = cache.kind;
        local->v.u64  = cache.v.u64;
        --st;
        cache.kind = st[0].kind;
        cache.v.u64  = st[0].v.u64;
        break;
      }
      HANDLE_CONST_INSTR(U32, u32);
      HANDLE_CONST_INSTR(S32, s32);
      HANDLE_CONST_INSTR(U64, u64);
      HANDLE_CONST_INSTR(S64, s64);
      HANDLE_CONST_INSTR(F32, f32);
      HANDLE_CONST_INSTR(F64, f64);

#define USUAL_BINOPS(type, field)               \
      HANDLE_BINOP_INSTR(type, field, +, ADD);  \
      HANDLE_BINOP_INSTR(type, field, -, SUB);  \
      HANDLE_BINOP_INSTR(type, field, *, MUL);  \
      HANDLE_BINOP_INSTR(type, field, /, DIV);

#define HANDLE_SHIFTS(type, field)                \
  HANDLE_BINOP_INSTR(type, field, <<, LSHIFT);    \
  HANDLE_BINOP_INSTR(type, field, >>, RSHIFT);

      USUAL_BINOPS(U32, u32);
      HANDLE_SHIFTS(U32, u32);
      HANDLE_BINOP_INSTR(U32, u32, %, REM);

      USUAL_BINOPS(S32, s32);
      HANDLE_SHIFTS(S32, s32);
      HANDLE_BINOP_INSTR(S32, s32, %, REM);

      USUAL_BINOPS(U64, u64);
      HANDLE_SHIFTS(U64, u64);
      HANDLE_BINOP_INSTR(U64, u64, %, REM);

      USUAL_BINOPS(S64, s64);
      HANDLE_SHIFTS(S64, s64);
      HANDLE_BINOP_INSTR(S64, s64, %, REM);

      USUAL_BINOPS(F32, f32);
      USUAL_BINOPS(F64, f64);
#undef USUAL_BINOPS
      default:
        LOG_ERROR("UNRECOGNIZED OPCODE (0x%x)", *(ip - 1));
    }
  }
outside:
  print_call_frame(frame);
  frame->st = st;
  frame->ip = ip;
  frame->cache = cache;
  return status;
}
