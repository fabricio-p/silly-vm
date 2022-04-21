#ifndef CODE_H
#define CODE_H
#include <c-ansi-sequences/graphics.h>
#include "silly.h"
#include "macros.h"

#ifdef DEBUG

enum STaggedValueKind {
#define T(t) SILLY_TYPE_##t
  T(VOID) = (0 << 2),
  T(U32)  = (1 << 2),
  T(S32)  = (2 << 2),
  T(U64)  = (3 << 2),
  T(S64)  = (4 << 2),
  T(F32)  = (5 << 2),
  T(F64)  = (6 << 2)
#undef T
};

enum SSecId {
#define S(s) SILLY_SEC_##s
  S(TYPE) = 1,
  S(DATA),
  S(FUNCTION),
  S(CODE)
#undef S
};
#else

/* Value types */
#define SILLY_TYPE_VOID (0 << 2)
#define SILLY_TYPE_U32  (1 << 2)
#define SILLY_TYPE_S32  (2 << 2)
#define SILLY_TYPE_U64  (3 << 2)
#define SILLY_TYPE_S64  (4 << 2)
#define SILLY_TYPE_F32  (5 << 2)
#define SILLY_TYPE_F64  (6 << 2)

/* Section ids */
#define SILLY_SEC_TYPE     1
#define SILLY_SEC_DATA     2
#define SILLY_SEC_FUNCTION 3
#define SILLY_SEC_CODE     4

#endif /* DEBUG */

/* Regular instructions  */
// The instruction set of the bytecode
#define SILLY_INSTR_NOP           0
#define SILLY_INSTR_HALT_VM       1
// Stack manipulation
#define SILLY_INSTR_DUPLICATE_ANY 2
#define SILLY_INSTR_DROP_ANY      3
// Constants
#define SILLY_INSTR_CONST_U32     4
#define SILLY_INSTR_CONST_S32     5
#define SILLY_INSTR_CONST_U64     6
#define SILLY_INSTR_CONST_S64     7
#define SILLY_INSTR_CONST_F32     8
#define SILLY_INSTR_CONST_F64     9
// Virtual registers
#define SILLY_INSTR_SET_LOCAL     10
#define SILLY_INSTR_GET_LOCAL     11
// Operations on 32 bit unsigned integers
#define SILLY_INSTR_ADD_U32       12
#define SILLY_INSTR_SUB_U32       13
#define SILLY_INSTR_MUL_U32       14
#define SILLY_INSTR_DIV_U32       15
#define SILLY_INSTR_REM_U32       16
#define SILLY_INSTR_LSHIFT_U32    17
#define SILLY_INSTR_RSHIFT_U32    18
// Operations on 32 bit signed integers
#define SILLY_INSTR_ADD_S32       19
#define SILLY_INSTR_SUB_S32       20
#define SILLY_INSTR_MUL_S32       21
#define SILLY_INSTR_DIV_S32       22
#define SILLY_INSTR_REM_S32       23
#define SILLY_INSTR_LSHIFT_S32    24
#define SILLY_INSTR_RSHIFT_S32    25
// Operations on 64 bit unsigned integers
#define SILLY_INSTR_ADD_U64       26
#define SILLY_INSTR_SUB_U64       27
#define SILLY_INSTR_MUL_U64       28
#define SILLY_INSTR_DIV_U64       29
#define SILLY_INSTR_REM_U64       30
#define SILLY_INSTR_LSHIFT_U64    31
#define SILLY_INSTR_RSHIFT_U64    32
// Operations on 64 bit signed integers
#define SILLY_INSTR_ADD_S64       33
#define SILLY_INSTR_SUB_S64       34
#define SILLY_INSTR_MUL_S64       35
#define SILLY_INSTR_DIV_S64       36
#define SILLY_INSTR_REM_S64       37
#define SILLY_INSTR_LSHIFT_S64    38
#define SILLY_INSTR_RSHIFT_S64    39
// Operations on 32 bit floating points
#define SILLY_INSTR_ADD_F32       40
#define SILLY_INSTR_SUB_F32       41
#define SILLY_INSTR_MUL_F32       42
#define SILLY_INSTR_DIV_F32       43
#define SILLY_INSTR_REM_F32       44
#define SILLY_INSTR_FLOOR_F32     45
#define SILLY_INSTR_CEIL_F32      46
// Operations on 64 bit floating points
#define SILLY_INSTR_ADD_F64       47
#define SILLY_INSTR_SUB_F64       48
#define SILLY_INSTR_MUL_F64       49
#define SILLY_INSTR_DIV_F64       50
#define SILLY_INSTR_REM_F64       51
#define SILLY_INSTR_FLOOR_F64     52
#define SILLY_INSTR_CEIL_F64      53
#define SILLY_INSTR_SQRT_F64      54

#define COUNT_SILLY_INSTR         55

#ifdef ENV_DEV
typedef struct InstrInfo {
  CStr  name;
  Ssize stack_arg_count;
  U8    *stack_arg_types;
  Ssize stack_result_count;
  U8    *stack_result_types;
  Ssize immediate_arg_count;
  U8    *immediate_arg_types;
} InstrInfo;

#define IINFO(instr) [INSTR(instr)] = 

static const InstrInfo instr_info[COUNT_SILLY_INSTR] = {
  IINFO(HALT_VM) {
    .name = "halt.vm",
    .stack_arg_count = 1,
    .stack_arg_types = (U8[]) { TYPE(S32) },
    .immediate_arg_count = 0
  },
  // Specialised for instructions that push constants into stack
#define CINFO(type, name_)                      \
IINFO(CONST_##type) {                           \
  .name = "const." name_,                       \
  .stack_arg_count = 0,                         \
  .stack_result_count = 0,                      \
  .immediate_arg_count = 1,                     \
  .immediate_arg_types = (U8[]) { TYPE(type) }  \
}
  CINFO(U32, "u32"),
  CINFO(S32, "s32"),
  CINFO(U64, "u64"),
  CINFO(S64, "s64"),
  CINFO(F32, "f32"),
  CINFO(F64, "f64"),
#undef CINFO
  // Specialised for instructions that are used as binary operators
#define BINFO(op, opname, type, typename)               \
IINFO(op##_##type) {                                    \
  .name = opname "." typename,                          \
  .stack_arg_count = 2,                                 \
  .stack_arg_types = (U8[]) { TYPE(type), TYPE(type) }, \
  .stack_result_count = 1,                              \
  .stack_result_types = (U8[]) { TYPE(type) },          \
  .immediate_arg_count = 0                              \
}
#define BINFO_ALL_OPS(type, typename) \
  BINFO(ADD, "add", type, typename),  \
  BINFO(SUB, "sub", type, typename),  \
  BINFO(MUL, "mul", type, typename),  \
  BINFO(DIV, "div", type, typename)

  BINFO_ALL_OPS(U32, "u32"),
  BINFO(LSHIFT, "lshift", U32, "u32"),
  BINFO(RSHIFT, "rshift", U32, "u32"),
  // BINFO(REM,    "rem",    U32, "u32"),

  BINFO_ALL_OPS(S32, "s32"),
  BINFO(LSHIFT, "lshift", S32, "s32"),
  BINFO(RSHIFT, "rshift", S32, "s32"),
  // BINFO(REM,    "rem",    S32, "u32"),

  BINFO_ALL_OPS(U64, "u64"),
  BINFO(LSHIFT, "lshift", U64, "u64"),
  BINFO(RSHIFT, "rshift", U64, "u64"),
  // BINFO(REM,    "rem",    U64, "u64"),

  BINFO_ALL_OPS(S64, "s64"),
  BINFO(LSHIFT, "lshift", S64, "s64"),
  BINFO(RSHIFT, "rshift", S64, "s64"),
  // BINFO(REM,    "rem",    S64, "u64"),

#define UINFO(op, opname, type, typename)       \
IINFO(op##_##type) {                            \
  .name = opname "." typename,                  \
  .stack_arg_count = 1,                         \
  .stack_arg_types = (U8[]) { TYPE(type) },     \
  .stack_result_count = 1,                      \
  .stack_result_types = (U8[]) { TYPE(type) },  \
  .immediate_arg_count = 0                      \
}
  BINFO_ALL_OPS(F32, "f32"),
  // UINFO(
  BINFO_ALL_OPS(F64, "f64")
#undef BINFO
#undef UINFO
};
__inline__
CStr type_name(U8 type)
{
#define TYPE_NAME_CASE(type, typename)        \
  case TYPE(type): name = typename; break

   CStr name = "unknown";
  switch (type)
  {
    TYPE_NAME_CASE(VOID, "void");
    TYPE_NAME_CASE(U32,  "u32");
    TYPE_NAME_CASE(S32,  "s32");
    TYPE_NAME_CASE(U64,  "u64");
    TYPE_NAME_CASE(S64,  "s64");
    TYPE_NAME_CASE(F32,  "f32");
    TYPE_NAME_CASE(F64,  "f64");
  }
#undef TYPE_NAME_CASE
  return name;
}
__inline__
void print_instruction(U8 instr)
{
#ifdef OUTCOLORS
#define OP_FMT(s) ANSISEQ_SETFG_MAGENTA s ANSISEQ_GR_RESET
#define TYPE_FMT(s) ANSISEQ_SETFG256(216) s ANSISEQ_GR_RESET
#define OPN_FMT(s)                                       \
  ANSISEQ_GR_SEQ(ANSISEQ_FG256(234) ANSISEQ_BG256(115))   \
  s ANSISEQ_GR_RESET
#else
#define OP_FMT(s) s
#define TYPE_FMT(s) s
#define OPN_FMT(s) s
#endif /* OUTCOLORS */
  if (instr >= COUNT_SILLY_INSTR)
  {
    LOG_ERROR("Unrecognized instruction, opcode: "OP_FMT("0x%x")"\n",
              instr);
  } else
  {
    InstrInfo const *const info = &instr_info[instr];
    if (info->name == NULL)
    {
      printf("(" OP_FMT("0x%02x") ") -> opcode lacks descriptor\n",
             instr);
      return;
    }
    printf(OPN_FMT("%s")"  \t(" OP_FMT("0x%02x") ") [",
           info->name, instr);
    if (info->stack_arg_count > 0)
    {
      printf(TYPE_FMT("%s"), type_name(info->stack_arg_types[0]));
      for (Ssize i = 1; i < info->stack_arg_count; ++i)
      {
        printf(", " TYPE_FMT("%s"), type_name(info->stack_arg_types[i]));
      }
    } else
    {
      printf(TYPE_FMT("void"));
    }
    printf("]    \t=>\t[");
    if (info->stack_result_count > 0)
    {
      printf(TYPE_FMT("%s"), type_name(info->stack_result_types[0]));
      for (Ssize i = 1; i < info->stack_result_count; ++i)
      {
        printf(", " TYPE_FMT("%s"),
               type_name(info->stack_result_types[i]));
      }
    } else
    {
      printf(TYPE_FMT("void"));
    }
    printf("]\t{");
    if (info->immediate_arg_count > 0)
    {
      printf(TYPE_FMT("%s"), type_name(info->immediate_arg_types[0]));
      for (Ssize i = 1; i < info->immediate_arg_count; ++i)
      {
        printf(", "TYPE_FMT("%s"),
               type_name(info->immediate_arg_types[i]));
      }
    }
    printf("}\n");
  }
#undef OP_FMT
#undef TYPE_FMT
#undef OPN_FMT
}
#else
#define print_instruction(instr_buf)
#endif /* ENV_DEV */

#endif /* CODE_H */
