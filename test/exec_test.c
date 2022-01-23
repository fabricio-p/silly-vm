#include "testing.h"
#include "macros.h"
#include "code.h"
#include "exec.h"
#include "internal.h"
#include "util.h"

void test_const(void)
{
  SValue stack_data[8] = {};
  SStack stack;
  SCallFrame frame;
  SFunc func;

  stack.size        = 8;
  stack.max_size    = 8;
  stack.frames      = &frame;
  stack.call_depth  = 1;

  frame.sl          = stack_data;
  frame.sb          = stack_data;
  frame.st          = stack_data;
  frame.function    = &func;
  frame.stack       = &stack;
  frame.frame_index = 0;
  frame.ip          = NULL;

  func.type = (SType *)(U16[]) { 0, 0 };
  func.code = (U8[]) {
    INSTR(CONST_U32), 0, 0, 0, 0,
    INSTR(CONST_S32), 0, 0, 0, 0,
    INSTR(CONST_U64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_S64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0
  };
  WRITE_U32(&func.code[1],  325432);
  WRITE_S32(&func.code[6],  -29191);
  WRITE_U64(&func.code[11], 4928277823);
  WRITE_S64(&func.code[20], -1849183);
  WRITE_F32(&func.code[29], 33.232);
  WRITE_F64(&func.code[34], 69.4819392);
  func.code_end = func.code + 42;
  func.name.len = 3;
  func.name.str = "foo";
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.st - frame.sb, 6);

  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(U32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].u32, 325432);

  CU_ASSERT_EQUAL_FATAL(frame.sb[1].kind, TYPE(S32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[1].s32, -29191);

  CU_ASSERT_EQUAL_FATAL(frame.sb[2].kind, TYPE(U64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[2].u64, 4928277823);

  CU_ASSERT_EQUAL_FATAL(frame.sb[3].kind, TYPE(S64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[3].s64, -1849183);

  CU_ASSERT_EQUAL_FATAL(frame.sb[4].kind, TYPE(F32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[4].f32, 33.232f);

  CU_ASSERT_EQUAL_FATAL(frame.sb[5].kind, TYPE(F64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[5].f64, 69.4819392);
  print_call_frame(&frame);
}
void test_add(void)
{
  SValue stack_data[8] = {};
  SStack stack;
  SCallFrame frame;
  SFunc func;

  stack.size        = 8;
  stack.max_size    = 8;
  stack.frames      = &frame;
  stack.call_depth  = 1;

  frame.sl          = stack_data;
  frame.sb          = stack_data;
  frame.st          = stack_data;
  frame.function    = &func;
  frame.stack       = &stack;
  frame.frame_index = 0;
  frame.ip          = NULL;

  func.name.len = 9;
  func.name.str = "anonymous";
  func.type = (SType *)(U16[]) { 0, 0 };
  func.code = (U8[]) {
    INSTR(CONST_U32), 0, 0, 0, 0,
    INSTR(CONST_U32), 0, 0, 0, 0,
    INSTR(ADD_U32)
  };
  func.code_end = func.code + 11;
  WRITE_U32(func.code + 1, 38199);
  WRITE_U32(func.code + 6, 2819427);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_EQUAL_FATAL(frame.st - frame.sb, 1);
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(U32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].u32, 38199 + 2819427);

  frame.st = frame.sb;
  func.code = (U8[]) {
    INSTR(CONST_S32), 0, 0, 0, 0,
    INSTR(CONST_S32), 0, 0, 0, 0,
    INSTR(ADD_S32)
  };
  func.code_end = func.code + 11;
  WRITE_S32(func.code + 1, -3917);
  WRITE_S32(func.code + 6, 3929);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame.st, &(frame.sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(S32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].s32, -3917 + 3929);

  frame.st = frame.sb;
  func.code = (U8[]) {
    INSTR(CONST_U64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_U64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(ADD_U64)
  };
  func.code_end = func.code + 19;
  WRITE_U64(func.code + 1, 38195619);
  WRITE_U64(func.code + 10, 199437957);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame.st, &(frame.sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(U64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].u64, 38195619ull + 199437957ull);

  frame.st = frame.sb;
  func.code = (U8[]) {
    INSTR(CONST_S64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_S64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(ADD_S64)
  };
  func.code_end = func.code + 19;
  WRITE_S64(func.code + 1, -39104782ll);
  WRITE_S64(func.code + 10, 39190ll);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame.st, &(frame.sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(S64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].s64, -39104782ll + 39190ll);

  frame.st = frame.sb;
  func.code = (U8[]) {
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(ADD_F32)
  };
  func.code_end = func.code + 11;
  WRITE_F32(func.code + 1, 38.188f);
  WRITE_F32(func.code + 6, 2.434f);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame.st, &(frame.sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(F32));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].f32, 38.188f + 2.434f);

  frame.st = frame.sb;
  func.code = (U8[]) {
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(ADD_F64)
  };
  func.code_end = func.code + 19;
  WRITE_F64(func.code + 1, 79.16493854);
  WRITE_F64(func.code + 10, 387.37398);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, &frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame.ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame.st, &(frame.sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].kind, TYPE(F64));
  CU_ASSERT_EQUAL_FATAL(frame.sb[0].f64, 79.16493854 + 387.37398);
}

int main(int argc, char **argv)
{
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "const.*", test_const },
    { "add.*",   test_add   },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "exec", NULL, NULL, NULL, NULL, tests },
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
