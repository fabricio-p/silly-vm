#ifndef OUTCOLORS
#define OUTCOLORS
#endif
#include "testing.h"
#include "parser.h"
#include "internal.h"
#include "code.h"
#include "macros.h"

// #if SILLY_LITTLE_ENDIAN == 0
#define ENC_U32(v)                                  \
  (((U32)(v)) >> 24), ((((U32)(v)) >> 16) & 0xff),  \
  ((((U32)(v)) >> 8) & 0xff), (((U32)(v)) & 0xff)
#define ENC_U16(v)                              \
  (((U16)(v)) & 0xff), ((((U16)(v)) >> 8) & 0xff)
/* #else
#define ENC_U32(v)                                  \
  (((U32)(v)) & 0xff), ((((U32)(v)) >> 8) & 0xff),  \
  ((((U32)(v)) >> 16) & 0xff), (((U32)(v)) >> 24)
#define ENC_U16(v)                              \
  (((U16)(v)) & 0xff), ((((U16)(v)) >> 8) & 0xff)
#endif */

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
  SEnv env = {
    // .type_pool_chain = NULL
  };
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
  SSecInfo info = {14, 1};
  U8 buffer[] = {
    ENC_U32(0), ENC_U32(0), ENC_U16(0), 3, 'f', 'o', 'o',
  };
  SEnv env;
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

int main(int argc, char **argv)
{
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "::type_section",     test_parse_type_section     },
    { "::function_section", test_parse_function_section },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "Parser", NULL, NULL, NULL, NULL, tests },
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
