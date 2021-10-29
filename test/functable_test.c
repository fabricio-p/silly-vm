#include "functable.h"
#include "testing.h"

void test_new(void)
{
  SFuncTable tbl = SFuncTable_new(6);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tbl);
  struct functable_hdr *header = functable_header(tbl);
  CU_ASSERT_EQUAL_FATAL(header->cap, 9);
  CU_ASSERT_EQUAL_FATAL(header->used, 0);
  SFuncTable_cleanup(tbl);
}

void test_insert(void)
{
  SFuncTable tbl = SFuncTable_new(1);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tbl);
  struct functable_hdr *header = functable_header(tbl);
  CU_ASSERT_EQUAL_FATAL(header->cap, 1);
  CU_ASSERT_EQUAL_FATAL(header->used, 0);
  char name[16];
  *(Usize *)name = strlen("main");
  sprintf(&(name[sizeof(Usize)]), "main");
  SFunc func = (SFunc) {
    .name = (PStr *)name
  };
  SFuncTable_insert(tbl, &func);
  CU_ASSERT_EQUAL_FATAL(header->used, 1);
  CU_ASSERT_PTR_EQUAL_FATAL(tbl[0], &func);
  SFuncTable_cleanup(tbl);
}

void test_get(void)
{
  SFuncTable tbl = SFuncTable_new(4);
  CU_ASSERT_PTR_NOT_NULL_FATAL(tbl);
  struct functable_hdr *header = functable_header(tbl);
  CU_ASSERT_EQUAL_FATAL(header->cap, 6);
  CU_ASSERT_EQUAL_FATAL(header->used, 0);
  char name[32];
  *(Usize *)name = strlen("a_function");
  sprintf(&(name[sizeof(Usize)]), "a_function");
  SFunc func = (SFunc) {
    .name = (PStr *)name
  };
  SFuncTable_insert(tbl, &func);
  CU_ASSERT_EQUAL_FATAL(header->used, 1);
  SFunc *funcp = SFuncTable_get(tbl, "a_function", *(Usize *)name);
  LOG_EVAL(*(Usize *)name, "u");
  LOG_EVAL(funcp, "p");
  CU_ASSERT_PTR_EQUAL_FATAL(funcp, &func);
  SFuncTable_cleanup(tbl);
}

int main(int argc, char **argv)
{
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "::new",    test_new    },
    { "::insert", test_insert },
    { "::get",    test_get    },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "SFuncTable", NULL, NULL, NULL, NULL, tests },
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
