#include "testing.h"
#include "parser.h"
#include "internal.h"
#include "code.h"
#include "macros.h"

#define ENC_U32(v)                                          \
  (U8)(((U32)(v)) & 0xff), (U8)((((U32)(v)) >> 8) & 0xff),  \
  (U8)((((U32)(v)) >> 16) & 0xff), (U8)(((U32)(v)) >> 24)
#define ENC_U16(v)                              \
  (U8)(((U16)(v)) & 0xff), (U8)((((U16)(v)) >> 8) & 0xff)

#define ENC_INFO(info) ENC_U32(info.size), ENC_U32(info.item_count)

void test_parse_type_section(void)
{
#define T(t) SILLY_TYPE_##t
  SSecInfo info = {18, 2};
  U8 buffer[] = {
    ENC_U16(3), ENC_U16(1),
    T(U32), T(F32), T(S64), T(F64),
    ENC_U16(4), ENC_U16(2),
    T(F64), T(F32), T(U64), T(S32), T(U32), T(S64)
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
  U8 types1[] = { T(U32), T(F32), T(S64), T(F64) };
  U8 types2[] = {
    T(F64), T(F32), T(U64), T(S32), T(U32), T(S64)
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
    ENC_U32(0), ENC_U32(0), ENC_U32(0), ENC_U16(0), 3, 'f', 'o', 'o',
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
  SSecInfo info = {61, 1}; // TODO: {61 + x, 2}
  U8 buffer[] = {
    OP(CONST_U32), ENC_U32(0),
    OP(CONST_U32), ENC_U32(1),
    OP(ADD_U32),
    OP(CONST_S32), ENC_U32(2),
    OP(CONST_S32), ENC_U32(3),
    OP(SUB_S32),
    OP(CONST_U64), ENC_U32(0),
    OP(CONST_U64), ENC_U32(1),
    OP(MUL_U64),
    OP(CONST_S64), ENC_U32(2),
    OP(CONST_S64), ENC_U32(3),
    OP(DIV_S64),
    OP(CONST_F32), ENC_U32(0),
    OP(CONST_F32), ENC_U32(1),
    OP(REM_F32),
    OP(CONST_F64), ENC_U32(0),
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
  mod.types[0].result_count = 6;
  mod.types[0].types = (U8[]) {
    T(U32), T(S32), T(U64), T(S64), T(F32), T(F64)
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
