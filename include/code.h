#ifndef CODE_H
#define CODE_H
#include <c-ansi-sequences/graphics.h>
#include "silly.h"
#include "macros.h"

#ifdef DEBUG

enum STaggedValueKind {
#define T(t) SILLY_TYPE_##t
  T(VOID) = (0 << 2),
  T(I32)  = (1 << 2),
  T(I64)  = (2 << 2),
  T(F32)  = (3 << 2),
  T(F64)  = (4 << 2)
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
#define SILLY_TYPE_I32  (1 << 2)
#define SILLY_TYPE_I64  (2 << 2)
#define SILLY_TYPE_F32  (3 << 2)
#define SILLY_TYPE_F64  (4 << 2)

/* Section ids */
#define SILLY_SEC_TYPE     1
#define SILLY_SEC_DATA     2
#define SILLY_SEC_FUNCTION 3
#define SILLY_SEC_CODE     4

#endif /* DEBUG */

/* INSTRUCTIONS START  */
#define SILLY_INSTR_NOP           0x00
#define SILLY_INSTR_HALT_VM       0x01
// Stack manipulation
#define SILLY_INSTR_DUPLICATE_ANY 0x02
#define SILLY_INSTR_DROP_ANY      0x03
// Constants
#define SILLY_INSTR_CONST_I32     0x04
#define SILLY_INSTR_CONST_I64     0x05
#define SILLY_INSTR_CONST_F32     0x06
#define SILLY_INSTR_CONST_F64     0x07
// Virtual registers
#define SILLY_INSTR_SET_LOCAL     0x08
#define SILLY_INSTR_GET_LOCAL     0x09
// Mathematical operations for 32 bit integers
#define SILLY_INSTR_ADD_I32       0x0a
#define SILLY_INSTR_SUB_I32       0x0b
#define SILLY_INSTR_MUL_I32       0x0c
#define SILLY_INSTR_DIV_U32       0x0d
#define SILLY_INSTR_REM_U32       0x0e
#define SILLY_INSTR_DIV_S32       0x0f
#define SILLY_INSTR_REM_S32       0x10
#define SILLY_INSTR_LSHIFT_I32    0x11
#define SILLY_INSTR_RSHIFT_I32    0x12
// Mathematical operations for 64 bit integers
#define SILLY_INSTR_ADD_I64       0x13
#define SILLY_INSTR_SUB_I64       0x14
#define SILLY_INSTR_MUL_I64       0x15
#define SILLY_INSTR_DIV_U64       0x16
#define SILLY_INSTR_REM_U64       0x17
#define SILLY_INSTR_DIV_S64       0x18
#define SILLY_INSTR_REM_S64       0x19
#define SILLY_INSTR_LSHIFT_I64    0x1a
#define SILLY_INSTR_RSHIFT_I64    0x1b
// Mathematical operations for 32 bit floating points
#define SILLY_INSTR_ADD_F32       0x1c
#define SILLY_INSTR_SUB_F32       0x1d
#define SILLY_INSTR_MUL_F32       0x1e
#define SILLY_INSTR_DIV_F32       0x1f
#define SILLY_INSTR_REM_F32       0x20
#define SILLY_INSTR_FLOOR_F32     0x21
#define SILLY_INSTR_CEIL_F32      0x22
#define SILLY_INSTR_SQRT_F32      0x23
// Mathematical operations for 64 bit floating points
#define SILLY_INSTR_ADD_F64       0x24
#define SILLY_INSTR_SUB_F64       0x25
#define SILLY_INSTR_MUL_F64       0x26
#define SILLY_INSTR_DIV_F64       0x27
#define SILLY_INSTR_REM_F64       0x28
#define SILLY_INSTR_FLOOR_F64     0x29
#define SILLY_INSTR_CEIL_F64      0x2a
#define SILLY_INSTR_SQRT_F64      0x2b
// Relational operators
//   32 bit integers
#define SILLY_INSTR_EQ_I32        0x2c
#define SILLY_INSTR_NEQ_I32       0x2d
#define SILLY_INSTR_EQZ_I32       0x2e
//     unsigned
#define SILLY_INSTR_GT_U32        0x2f
#define SILLY_INSTR_LT_U32        0x30
#define SILLY_INSTR_GE_U32        0x31
#define SILLY_INSTR_LE_U32        0x32
//     signed
#define SILLY_INSTR_GT_S32        0x33
#define SILLY_INSTR_LT_S32        0x34
#define SILLY_INSTR_GE_S32        0x35
#define SILLY_INSTR_LE_S32        0x36
//   64 bit integers
#define SILLY_INSTR_EQ_I64        0x37
#define SILLY_INSTR_NEQ_I64       0x38
#define SILLY_INSTR_EQZ_I64       0x39
//     unsigned
#define SILLY_INSTR_GT_U64        0x3a
#define SILLY_INSTR_LT_U64        0x3b
#define SILLY_INSTR_GE_U64        0x3c
#define SILLY_INSTR_LE_U64        0x3d
//     signed
#define SILLY_INSTR_GT_S64        0x3e
#define SILLY_INSTR_LT_S64        0x3f
#define SILLY_INSTR_GE_S64        0x40
#define SILLY_INSTR_LE_S64        0x41
//   32 bit floats
#define SILLY_INSTR_EQ_F32        0x42
#define SILLY_INSTR_NEQ_F32       0x43
#define SILLY_INSTR_EQZ_F32       0x44
#define SILLY_INSTR_GT_F32        0x45
#define SILLY_INSTR_LT_F32        0x46
#define SILLY_INSTR_GE_F32        0x47
#define SILLY_INSTR_LE_F32        0x48
//   64 bit floats
#define SILLY_INSTR_EQ_F64        0x49
#define SILLY_INSTR_NEQ_F64       0x4a
#define SILLY_INSTR_EQZ_F64       0x4b
#define SILLY_INSTR_GT_F64        0x4c
#define SILLY_INSTR_LT_F64        0x4d
#define SILLY_INSTR_GE_F64        0x4e
#define SILLY_INSTR_LE_F64        0x4f
#define COUNT_SILLY_INSTR         0x050
/* INSTRUCTIONS END */

#ifdef ENV_DEV

#define SILLY_TYPE_U32 SILLY_TYPE_I32
#define SILLY_TYPE_S32 SILLY_TYPE_I32
#define SILLY_TYPE_U64 SILLY_TYPE_I64
#define SILLY_TYPE_S64 SILLY_TYPE_I64
typedef struct InstrInfo {
  CStr  name;
  Ssize stack_arg_count;
  U8    *stack_arg_types;
  Ssize stack_result_count;
  U8    *stack_result_types;
  Ssize immediate_arg_count;
  U8    *immediate_arg_types;
} InstrInfo;

#define IINFO(instr) [INSTR(instr)] = (InstrInfo) 

static const InstrInfo instr_info[COUNT_SILLY_INSTR] = {
  IINFO(NOP) {
    .name = "nop",
    .stack_arg_count = 0,
    .immediate_arg_count = 0
  },
  IINFO(HALT_VM) {
    .name = "halt.vm",
    .stack_arg_count = 1,
    .stack_arg_types = (U8[]) { TYPE(I32) },
    .immediate_arg_count = 0
  },
  IINFO(DUPLICATE_ANY) {
    .name = "duplicate.any",
    .stack_arg_count = 1,
    .stack_arg_types = (U8[]) { TYPE(F64 + 1) },
    .immediate_arg_count = 0,
    .stack_result_count = 2,
    .stack_result_types = (U8[]) { TYPE(F64 + 1), TYPE(F64 + 1) },
  },
  IINFO(DROP_ANY) {
    .name = "drop.any",
    .stack_arg_count = 1,
    .stack_arg_types = (U8[]) { TYPE(F64 + 1) },
    .stack_result_count = 0,
    .immediate_arg_count = 0
  },
  // Specialised for instructions that push constants into stack
#define CINFO(type, name_)                        \
  IINFO(CONST_##type) {                           \
    .name = "const." name_,                       \
    .stack_arg_count = 0,                         \
    .stack_result_count = 1,                      \
    .stack_result_types = (U8[]) { TYPE(type) },  \
    .immediate_arg_count = 1,                     \
    .immediate_arg_types = (U8[]) { TYPE(type) }  \
  }
  CINFO(I32, "i32"),
  CINFO(I64, "i64"),
  CINFO(F32, "f32"),
  CINFO(F64, "f64"),
#undef CINFO
  IINFO(SET_LOCAL) {
    .name = "set.local",
    .stack_arg_count = 1,
    .stack_arg_types = (U8[]) { TYPE(F64 + 1) },
    .stack_result_count = 0,
    .immediate_arg_count = 1,
    .immediate_arg_types = (U8[]) { TYPE(I32) }
  },
  IINFO(GET_LOCAL) {
    .name = "get.local",
    .stack_arg_count = 0,
    .stack_result_count = 1,
    .stack_result_types = (U8[]) { TYPE(F64 + 1) },
    .immediate_arg_count = 1,
    .immediate_arg_types = (U8[]) { TYPE(I32) }
  },
  // Specialised for instructions that are used as binary operators
#define BINFO(op, opname, type, typename)                 \
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
  BINFO(MUL, "mul", type, typename)

  BINFO_ALL_OPS(I32, "i32"),
  BINFO(REM, "rem", U32, "u32"),
  BINFO(REM, "rem", S32, "s32"),
  BINFO(LSHIFT, "lshift", I32, "i32"),
  BINFO(RSHIFT, "rshift", I32, "i32"),

  BINFO_ALL_OPS(I64, "i64"),
  BINFO(REM, "rem", U64, "u64"),
  BINFO(REM, "rem", S64, "s64"),
  BINFO(LSHIFT, "lshift", I64, "i64"),
  BINFO(RSHIFT, "rshift", I64, "i64"),

#define UINFO(op, opname, type, typename)         \
  IINFO(op##_##type) {                            \
    .name = opname "." typename,                  \
    .stack_arg_count = 1,                         \
    .stack_arg_types = (U8[]) { TYPE(type) },     \
    .stack_result_count = 1,                      \
    .stack_result_types = (U8[]) { TYPE(type) },  \
    .immediate_arg_count = 0                      \
  }

#define UINFO_FXX(op, opn, p) UINFO(op, opn, F##p, "f"#p)

  BINFO_ALL_OPS(F32, "f32"),
  UINFO_FXX(FLOOR, "floor", 32),
  UINFO_FXX(CEIL, "ceil", 32),
  BINFO_ALL_OPS(F64, "f64"),
  UINFO_FXX(FLOOR, "floor", 64),
  UINFO_FXX(CEIL, "ceil", 64),
  UINFO_FXX(SQRT, "sqrt", 64),

#undef UINFO_FXX
#undef BINFO

#define CMPINFO(op, opn, it, t, tn)                 \
  IINFO(op##_##it) {                                \
    .name = opn "." tn,                             \
    .stack_arg_count = 2,                           \
    .stack_arg_types = (U8[]) { TYPE(t), TYPE(t) }, \
    .stack_result_count = 1,                        \
    .stack_result_types = (U8[]) { TYPE(U32) },     \
    .immediate_arg_count = 0                        \
  }
#define CMPINFO2(op, opn, t, tn) CMPINFO(op, opn, t, t, tn)

  CMPINFO(EQ,  "eq",  I32, S32 + 1, "i32"),
  CMPINFO(NEQ, "neq", I32, S32 + 1, "i32"),
  CMPINFO(EQZ, "eqz", I32, S32 + 1, "i32"),

  CMPINFO2(GT, "gt", U32, "u32"),
  CMPINFO2(LT, "lt", U32, "u32"),
  CMPINFO2(GE, "ge", U32, "u32"),
  CMPINFO2(LE, "le", U32, "u32"),

  CMPINFO2(GT, "gt", S32, "s32"),
  CMPINFO2(LT, "lt", S32, "s32"),
  CMPINFO2(GE, "ge", S32, "s32"),
  CMPINFO2(LE, "le", S32, "s32"),


  CMPINFO(EQ,  "eq",  I64, S64 + 1, "i64"),
  CMPINFO(NEQ, "neq", I64, S64 + 1, "i64"),
  CMPINFO(EQZ, "eqz", I64, S64 + 1, "i64"),

  CMPINFO2(GT, "gt", U64, "u64"),
  CMPINFO2(LT, "lt", U64, "u64"),
  CMPINFO2(GE, "ge", U64, "u64"),
  CMPINFO2(LE, "le", U64, "u64"),

  CMPINFO2(GT, "gt", S64, "s64"),
  CMPINFO2(LT, "lt", S64, "s64"),
  CMPINFO2(GE, "ge", S64, "s64"),
  CMPINFO2(LE, "le", S64, "s64"),


  CMPINFO2(EQ,  "eq",  F32, "f32"),
  CMPINFO2(NEQ, "neq", F32, "f32"),
  CMPINFO2(EQZ, "eqz", F32, "f32"),
  CMPINFO2(GT,  "gt",  F32, "f32"),
  CMPINFO2(LT,  "lt",  F32, "f32"),
  CMPINFO2(GE,  "ge",  F32, "f32"),
  CMPINFO2(LE,  "le",  F32, "f32"),


  CMPINFO2(EQ,  "eq",  F64, "f64"),
  CMPINFO2(NEQ, "neq", F64, "f64"),
  CMPINFO2(EQZ, "eqz", F64, "f64"),
  CMPINFO2(GT,  "gt",  F64, "f64"),
  CMPINFO2(LT,  "lt",  F64, "f64"),
  CMPINFO2(GE,  "ge",  F64, "f64"),
  CMPINFO2(LE,  "le",  F64, "f64"),

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
    TYPE_NAME_CASE(I32, "i32");
    TYPE_NAME_CASE(I64, "i64");
    TYPE_NAME_CASE(F32,  "f32");
    TYPE_NAME_CASE(F64,  "f64");
    TYPE_NAME_CASE(F64 + 1, "%");
  }
#undef TYPE_NAME_CASE
  return name;
}

static void print_instruction(U8 instr)
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
    printf(OPN_FMT("%s") "  %s(" OP_FMT("0x%02x") ") [",
           info->name, instr == 0 ? "\t\t" : "\t", instr);
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
    printf("]      \t=>\t[");
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
    putchar(']');
    // printf("]      \t{");
    /* if (info->immediate_arg_count > 0)
    {
      printf(TYPE_FMT("%s"), type_name(info->immediate_arg_types[0]));
      for (Ssize i = 1; i < info->immediate_arg_count; ++i)
      {
        printf(", "TYPE_FMT("%s"),
               type_name(info->immediate_arg_types[i]));
      }
    } */
    putchar('\n');
    // printf("}\n");
  }
#undef OP_FMT
#undef TYPE_FMT
#undef OPN_FMT
}
#else
#define print_instruction(instr_buf)
#endif /* ENV_DEV */

#endif /* CODE_H */
