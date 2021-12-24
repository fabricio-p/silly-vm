#include "testing.h"
#include "parser.h"
#include "internal.h"
#include "code.h"
#include "macros.h"

void test_parse_type_section(void)
{
  U8 buffer[64];
  SEnv env = {
    .type_pool_chain = NULL
  };
  SModule mod = {
    .path = "",
    .raw = {
      .buffer = buffer,
      .type_sec_size = sizeof(U16) * 4 + 10
    },
    .loaded = {
      .types_buffer = NULL
    },
    .types = STypeVec_with_capacity(2)
  };
  voidptr ptr = buffer;
  memcpy(ptr, "\0SillyVM", 8);
  ptr += 8;
  STORE_U32(ptr, mod.raw.type_sec_size);
  ptr += sizeof(U32) * 4;
  STORE_U16(ptr, 3);
  ptr += sizeof(U16);
  STORE_U16(ptr, 1);
  ptr += sizeof(U16);
#define T(t) SILLY_TYPE_##t
  U8 types1[] = { T(U32), T(F32), T(S64), T(F64) };
  memcpy(ptr, types1, sizeof(types1));
  ptr += sizeof(types1);

  STORE_U16(ptr, 4);
  ptr += sizeof(U16);
  STORE_U16(ptr, 2);
  ptr += sizeof(U16);
  U8 types2[] = {
    T(F64), T(F32), T(U64), T(S32), T(U32), T(S64)
  };
  memcpy(ptr, types2, sizeof(types2));
  ptr += sizeof(types2);
#undef T
  CU_ASSERT_EQUAL_FATAL(
    parse_type_section(&env, &mod, &(buffer[8 + sizeof(U32) * 4]),
                       mod.raw.type_sec_size),
    SILLY_E_OK
  );
  voidptr type_p = mod.loaded.types_buffer;
  CU_ASSERT_PTR_NOT_NULL_FATAL(type_p);
  LOG_EVAL(mod.types[0], "p");
  LOG_EVAL(mod.types[1], "p");
  LOG_EVAL(type_p, "p");
  LOG_EVAL(buffer, "p");
  CU_ASSERT_EQUAL_FATAL(mod.types[0], type_p);
  CU_ASSERT_EQUAL_FATAL(mod.types[1], type_p + offsetof(SType, types) + 4);
  CU_ASSERT_EQUAL_FATAL(mod.types[0]->param_count, 3);
  CU_ASSERT_EQUAL_FATAL(mod.types[0]->result_count, 1);
  for (Int i = 0; i < 4; ++i)
  {
    // TODO: Look at this tomorrow
    LOG_DEBUG("%s, %s\n", type_name(types1[i]),
              type_name(mod.types[0]->types[i]));
  }
  CU_ASSERT_FATAL(memcmp(mod.types[0]->types, types1, 4) == 0);
  CU_ASSERT_EQUAL_FATAL(mod.types[1]->param_count, 4);
  CU_ASSERT_EQUAL_FATAL(mod.types[1]->result_count, 2);
  CU_ASSERT_FATAL(memcmp(mod.types[1]->types, types2, 6) == 0);
}

int main(int argc, char **argv)
{
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "::type_section", test_parse_type_section },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "bc_parse", NULL, NULL, NULL, NULL, tests },
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
