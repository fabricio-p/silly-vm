#include <math.h>
#include <c-ansi-sequences/graphics.h>
#include "silly.h"
#include "macros.h"
#include "exec.h"
#include "code.h"
#include "internal.h"
#include "util.h"
#define HANDLE_INSTR(instr) case SILLY_INSTR_##instr:
typedef U32 I32;
typedef U64 I64;

#define push(val, type, field)        \
  do {                                \
    st->v.field = (val);              \
    st->kind = (type);                \
    ++st;                             \
    cache_##field = (val);            \
    cache_kind = (type);              \
  } while(0)

#define HANDLE_BINOP_INSTR(type, field, op, opname)   \
  HANDLE_INSTR(opname##_##type)                       \
  {                                                   \
    --st;                                             \
    st[-1].v.field = st[-1].v.field op##=             \
                      cache_##field;                  \
  } break;

#define HANDLE_UNOP_INSTR(type, field, opname, action)  \
  HANDLE_INSTR(opname##_##type)                         \
  {                                                     \
    --st;                                               \
    st->v.field = cache_##field = (action);             \
  } break;

#define HALT_E_CREATE(exit_code)                      \
  (((exit_code) << (sizeof(U8) * 8)) | SILLY_E_HALT)

SStatus SFunc_exec(SEnv *env, SCallFrame *frame, SCPool pools[4])
{
  SStatus status = SILLY_E_OK;
  U8 const *ip = frame->function->code;
  U8 const *const end = frame->function->code_end;
  STaggedValueKind cache_kind = SILLY_TYPE_VOID;
  register I32 cache_i32 = 0;
  register I64 cache_i64 = 0;
#define cache_u32 cache_i32
#define cache_s32 cache_i32
#define cache_u64 cache_i64
#define cache_s64 cache_i64
  register F32 cache_f32 = 0;
  register F64 cache_f64 = 0;
  STaggedValue *sl = frame->sl;
  // STaggedValue *sb = frame->sb;
  STaggedValue *st = frame->st;
  for (; ip < end;)
  {
#ifdef ENV_DEV
#ifdef OUTCOLORS
#define IP_FMT(s) ANSIQ_SETFG256(154) s ANSIQ_GR_RESET
#else
#define IP_FMT(s) s
#endif /* OUTCOLORS */
    printf(
        "[ip: " IP_FMT("0x%08x") "] ",
        (unsigned int)(ip - frame->function->code)
    );
#endif
    print_instruction(*ip);
    U8 instruction = *(ip++);
    switch (instruction)
    {
      HANDLE_INSTR(NOP)
      {
        break;
      }
      HANDLE_INSTR(HALT_VM)
      {
        S32 exit_code = cache_s32;
        --st;
        status = HALT_E_CREATE(exit_code);
        goto done;
      }
      HANDLE_INSTR(DUPLICATE_ANY)
      {
        st[1].kind = cache_kind;
        st[1].v.u64  = cache_u64;
        ++st;
        break;
      }
      HANDLE_INSTR(DROP_ANY)
      {
        --st;
        cache_kind = st[0].kind;
        cache_u64  = st[0].v.u64;
        break;
      }
      HANDLE_INSTR(GET_LOCAL)
      {
        ++st;
        U16 index = READ_U16(ip);
        ip += sizeof(U16);
        STaggedValue *local = &(sl[index]);
        st[0].kind = cache_kind = local->kind;
        st[0].v.u64  = cache_u64  = local->v.u64;
        break;
      }
      HANDLE_INSTR(SET_LOCAL)
      {
        U16 index = READ_U16(ip);
        ip += sizeof(U16);
        STaggedValue *local = &(sl[index]);
        local->kind = cache_kind;
        local->v.u64  = cache_u64;
        --st;
        cache_kind = st[0].kind;
        cache_u64  = st[0].v.u64;
        break;
      }
      HANDLE_INSTR(CONST_I32)
      HANDLE_INSTR(CONST_I64)
      HANDLE_INSTR(CONST_F32)
      HANDLE_INSTR(CONST_F64)
      {
        static Uint const sizeof_kind[] = {
          sizeof(I32), sizeof(I64), sizeof(F32), sizeof(F64)
        };
        U32 i = LOAD_U32(ip);
        ip += sizeof(U32);
        Uint pooli = instruction - SILLY_INSTR_CONST_I32;
        cache_kind = (pooli + 1) << 2;
        st->kind = cache_kind; 
        U8 *lit_addr = pools[pooli].data + i * sizeof_kind[pooli];
        switch (cache_kind)
        {
          case SILLY_TYPE_I32:
            st->v.u32 = cache_i32 = *(I32 *)lit_addr; break;
          case SILLY_TYPE_I64:
            st->v.u64 = cache_i64 = *(I64 *)lit_addr; break;
          case SILLY_TYPE_F32:
            st->v.f32 = cache_f32 = *(F32 *)lit_addr; break;
          case SILLY_TYPE_F64:
            st->v.f64 = cache_f64 = *(F64 *)lit_addr; break;
        }
        ++st;
        break;
      }

#define USUAL_BINOPS(type, field)               \
      HANDLE_BINOP_INSTR(type, field, +, ADD);  \
      HANDLE_BINOP_INSTR(type, field, -, SUB);  \
      HANDLE_BINOP_INSTR(type, field, *, MUL);  \

#define HANDLE_SHIFTS(type, field)                \
  HANDLE_BINOP_INSTR(type, field, <<, LSHIFT);    \
  HANDLE_BINOP_INSTR(type, field, >>, RSHIFT);

      // TODO: Info for s and u integer div
      USUAL_BINOPS(I32,  u32);
      HANDLE_SHIFTS(I32, u32);
      HANDLE_BINOP_INSTR(U32, u32, /, DIV);
      HANDLE_BINOP_INSTR(S32, s32, /, DIV);
      HANDLE_BINOP_INSTR(U32, u32, %, REM);
      HANDLE_BINOP_INSTR(S32, s32, %, REM);

      USUAL_BINOPS(I64,  u64);
      HANDLE_SHIFTS(I64, u64);
      HANDLE_BINOP_INSTR(U64, u64, /, DIV);
      HANDLE_BINOP_INSTR(S64, s64, /, DIV);
      HANDLE_BINOP_INSTR(U64, u64, %, REM);
      HANDLE_BINOP_INSTR(S64, s64, %, REM);

      USUAL_BINOPS(F32, f32);
      HANDLE_INSTR(REM_F32)
      {
        st -= 2;
        st->v.f32 = cache_f32 = fmodf(st[0].v.f32, cache_f32);
        break;
      }
      HANDLE_UNOP_INSTR(F32, f32, FLOOR, floorf(cache_f32));
      HANDLE_UNOP_INSTR(F32, f32, CEIL,  ceilf (cache_f32));
      HANDLE_UNOP_INSTR(F32, f32, SQRT,  sqrtf(cache_f32));
      USUAL_BINOPS(F64, f64);
      HANDLE_INSTR(REM_F64)
      {
        st -= 2;
        st->v.f64 = cache_f64 = fmod(st[0].v.f64, cache_f64);
        break;
      }
      HANDLE_UNOP_INSTR(F64, f64, FLOOR, floor(cache_f64));
      HANDLE_UNOP_INSTR(F64, f64, CEIL,  ceil (cache_f64));
      HANDLE_UNOP_INSTR(F64, f64, SQRT,  sqrt (cache_f64));
#undef USUAL_BINOPS
      default:
        LOG_ERROR("UNRECOGNIZED OPCODE (0x%x)", *(ip - 1));
        status = SILLY_E_INVOPC;
        goto done;
    }
  }
done:
  print_call_frame(frame);
  frame->st = st;
  frame->ip = ip;
  frame->cache.kind = cache_kind;
  switch (cache_kind)
  {
    case SILLY_TYPE_I32:
      frame->cache.v.u32 = cache_i32; break;
    case SILLY_TYPE_I64:
      frame->cache.v.u64 = cache_i64; break;
    case SILLY_TYPE_F32:
      frame->cache.v.f32 = cache_f32; break;
    case SILLY_TYPE_F64:
      frame->cache.v.f64 = cache_f64; break;
  }
  return status;
}
