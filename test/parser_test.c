#include <limits.h>
#include "testing.h"
#include "parser.h"
#include "internal.h"
#include "code.h"
#include "macros.h"

typedef U32 I32;
typedef U64 I64;

#define ENC_I64(v)                                                  \
  (U8)(((I64)(v)) & 0xff), (U8)((((I64)(v)) >> 8) & 0xff),          \
  (U8)((((I64)(v)) >> 16) & 0xff), (U8)((((I64)(v)) >> 24) & 0xff), \
  (U8)((((I64)(v)) >> 32) & 0xff), (U8)((((I64)(v)) >> 40) & 0xff), \
  (U8)((((I64)(v)) >> 48) & 0xff), (U8)((((I64)(v)) >> 54) & 0xff)

#define ENC_I32(v)                                          \
  (U8)(((I32)(v)) & 0xff), (U8)((((I32)(v)) >> 8) & 0xff),  \
  (U8)((((I32)(v)) >> 16) & 0xff), (U8)(((I32)(v)) >> 24)
#define ENC_U16(v)                              \
  (U8)(((U16)(v)) & 0xff), (U8)((((U16)(v)) >> 8) & 0xff)

#define CTV(t, ...) (t) {__VA_ARGS__}
#define ENC_F64(v) ENC_I32((*(I64 *)&CTV(F64, v)) & ULONG_MAX),         \
                   ENC_I32(((*(I64 *)&CTV(F64, v)) >> 32) & ULONG_MAX)
#define ENC_F32(v) ENC_I32(*(I32 *)&CTV(F32, v))

#define ENC_INFO(info) ENC_I32(info.size), ENC_I32(info.item_count)

#include <inttypes.h>

void test_parse_cpool_section(void)
{
  U8 const buffer[] = {
    // i64 count, f64 count, i32 count, f32 count
    ENC_I32(2), ENC_I32(2), ENC_I32(2), ENC_I32(2),
    // i64
    ENC_I64(383455615655ull), ENC_I64(-47284655182ll),
    // f64
    ENC_F64(43.43543), ENC_F64(-3343.2344),
    // i32
    ENC_I32(356555ul), ENC_I32(-4521l),
    // f32
    ENC_F32(24.23f), ENC_F32(-232.4f)
  };
  SEnv env = {0};
  SModule mod = {0};
  CU_ASSERT_EQUAL(
      parse_cpool_section(&env, &mod, buffer, CTV(SSecInfo, 0)),
      SILLY_E_OK
  );
  CU_ASSERT_EQUAL(mod.cpools.i64.size, 2);
  CU_ASSERT_EQUAL(mod.cpools.f64.size, 2);
  CU_ASSERT_EQUAL(mod.cpools.i32.size, 2);
  CU_ASSERT_EQUAL(mod.cpools.f32.size, 2);

  CU_ASSERT_EQUAL(mod.cpools.i64.data[0], 383455615655ull);
  CU_ASSERT_EQUAL(mod.cpools.i64.data[1], (I64)-47284655182ll);

  CU_ASSERT_EQUAL(mod.cpools.f64.data[0], 43.43543);
  CU_ASSERT_EQUAL(mod.cpools.f64.data[1], -3343.2344);

  CU_ASSERT_EQUAL(mod.cpools.i32.data[0], 356555ul);
  CU_ASSERT_EQUAL(mod.cpools.i32.data[1], (I32)-4521l);

  CU_ASSERT_EQUAL(mod.cpools.f32.data[0], 24.23f);
  CU_ASSERT_EQUAL(mod.cpools.f32.data[1], -232.4f);
}

void test_parse_type_section(void)
{
#define T(t) SILLY_TYPE_##t
  SSecInfo info = {18, 2};
  U8 buffer[] = {
    ENC_U16(3), ENC_U16(1),
    T(I32), T(F32), T(I64), T(F64),
    ENC_U16(4), ENC_U16(2),
    T(F64), T(F32), T(I64), T(I32), T(I32), T(I64)
  };
  SEnv env = {0};
  SModule mod = {
    .path = "",
    .raw = {
      .seci.type = info
    },
    .types = STypeVec_with_capacity(info.item_count)
    /* .loaded = {
      // .types_buffer = NULL
    }, */
  };
  U8 types1[] = { T(I32), T(F32), T(I64), T(F64) };
  U8 types2[] = {
    T(F64), T(F32), T(I64), T(I32), T(I32), T(I64)
  };
  CU_ASSERT_EQUAL_FATAL(
      parse_type_section(&env, &mod, buffer, info),
      SILLY_E_OK
  );
  CU_ASSERT_EQUAL_FATAL(STypeVec_len(mod.types), 2);
  CU_ASSERT_EQUAL_FATAL(mod.types[0].param_count, 3);
  CU_ASSERT_EQUAL_FATAL(mod.types[0].result_count, 1);
  CU_ASSERT_FATAL(memcmp(mod.types[0].types, types1, 4) == 0);
  CU_ASSERT_EQUAL_FATAL(mod.types[1].param_count, 4);
  CU_ASSERT_EQUAL_FATAL(mod.types[1].result_count, 2);
  CU_ASSERT_FATAL(memcmp(mod.types[1].types, types2, 6) == 0);

  STypeVec_cleanup(mod.types);
}

void test_parse_function_section(void)
{
#define OP(op) SILLY_INSTR_##op
  SSecInfo info = {18, 1};
  U8 buffer[] = {
    ENC_I32(0), ENC_I32(0), ENC_I32(0), ENC_U16(0), 3, 'f', 'o', 'o',
  };
  SEnv env = {0};
  SModule mod = {
    .path = "",
    .raw.seci.func = info,
    .types = STypeVec_with_length(1),
    .functions = SFuncVec_with_capacity(info.item_count)
  };
  CU_ASSERT_EQUAL_FATAL(
      parse_function_section(&env, &mod, buffer, info),
      SILLY_E_OK
  );
  CU_ASSERT_EQUAL_FATAL(SFuncVec_len(mod.functions), 1);
  CU_ASSERT_EQUAL_FATAL(mod.functions[0].raw.code_offset, 0);
  CU_ASSERT_EQUAL_FATAL(mod.functions[0].raw.type_idx, 0);
  CU_ASSERT_EQUAL_FATAL(mod.functions[0].raw.offset, 0);
  CU_ASSERT_EQUAL_FATAL(mod.functions[0].name.len, 3);
  CU_ASSERT_FATAL(memcmp(mod.functions[0].name.str, "foo", 3) == 0);

  STypeVec_cleanup(mod.types);
  SFuncVec_cleanup(mod.functions);
}

void test_parse_code_section(void)
{
#define T(t) SILLY_TYPE_##t
#define OP(op) SILLY_INSTR_##op
  SSecInfo info = {39, 1};
  U8 buffer[] = {
    OP(CONST_I32), ENC_I32(2),
    OP(CONST_I32), ENC_I32(3),
    OP(SUB_I32),
    OP(CONST_I64), ENC_I32(2),
    OP(CONST_I64), ENC_I32(3),
    OP(DIV_S64),
    OP(CONST_F32), ENC_I32(0),
    OP(CONST_F32), ENC_I32(1),
    OP(REM_F32),
    OP(CONST_F64), ENC_I32(0),
    OP(SQRT_F64)
  };
  SEnv env = {0};
  SModule mod = {
    .raw.seci.code = info,
    .types = STypeVec_with_length(1),
    .functions = SFuncVec_with_length(1),
  };
  for (Int i = 0; i < 4; ++i)
  {
    mod.cpool_arr[i].size = 4;
  }
  mod.types[0].param_count = 0;
  mod.types[0].result_count = 4;
  mod.types[0].types = (U8[]) {
    T(I32), T(I64), T(F32), T(F64)
  };
  mod.functions[0].type = &(mod.types[0]);
  mod.functions[0].locals.count = 0;
  mod.functions[0].raw.code_offset = 0;
  mod.functions[0].raw.code_size = info.size;
  mod.functions[0].raw.type_idx = 0;

  CU_ASSERT_EQUAL_FATAL(parse_code_section(&env, &mod, buffer, info),
                        SILLY_E_OK);

  STypeVec_cleanup(mod.types);
  SFuncVec_cleanup(mod.functions);
#undef T
#undef OP
}

int main(int argc, char **argv)
{
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "::cpool",     test_parse_cpool_section   },
    { "::type",     test_parse_type_section     },
    { "::function", test_parse_function_section },
    { "::code",     test_parse_code_section     },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "SectionParser", NULL, NULL, NULL, NULL, tests },
    CU_SUITE_INFO_NULL
  };
  CU_initialize_registry();
  if (CU_register_suites(suites) != CUE_SUCCESS &&
      (status = EXIT_FAILURE))
    goto cleanup;
  RUN_TESTS;
cleanup:
  CU_cleanup_registry();
  return status;
}
