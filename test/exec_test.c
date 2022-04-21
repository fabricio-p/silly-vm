#include <stdint.h>
#include "testing.h"
#include "macros.h"
#include "code.h"
#include "exec.h"
#include "internal.h"
#include "util.h"

#define enc_i32(n) ((n) & 0xff), (((n) >> 8) & 0xff),         \
                   (((n) >> 16) & 0xff), (((n) >> 24) & 0xff)
#define enc_i64(n) enc_i32((U32)(((U64)(n)) & UINT32_MAX)),           \
                   enc_i32((U32)((((U64)(n)) >> 32) & UINT32_MAX))

void test_const(void)
{
  SStack stack;
  SCallFrame *frame = &(stack.frames[0]);
  SFunc func;

  stack.frame      = frame;

  frame->sl          = stack.data;
  frame->sb          = stack.data;
  frame->st          = stack.data;
  frame->function    = &func;
  frame->stack       = &stack;
  frame->frame_index = 0;
  frame->ip          = NULL;
#define strify(expr) #expr

  // TODO: fix the typing shit
  func.type = &(SType) { 0, 0, NULL };
  func.code = (U8[]) {
    INSTR(CONST_U32), enc_i32(325432UL),
    INSTR(CONST_S32), enc_i32(-29191L),
    INSTR(CONST_U64), enc_i64(4928277823ULL),
    INSTR(CONST_S64), enc_i64(-1849183LL),
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0
  };
  WRITE_F32(&func.code[29], 33.232F);
  WRITE_F64(&func.code[34], 69.4819392);
  func.code_end = func.code + 42;
  func.name = (SizedStr) {"foo", 3};
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->st - frame->sb, 6);

  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(U32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.u32, 325432UL);

  CU_ASSERT_EQUAL_FATAL(frame->sb[1].kind, TYPE(S32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[1].v.s32, -29191L);

  CU_ASSERT_EQUAL_FATAL(frame->sb[2].kind, TYPE(U64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[2].v.u64, 4928277823ULL);

  CU_ASSERT_EQUAL_FATAL(frame->sb[3].kind, TYPE(S64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[3].v.s64, -1849183LL);

  CU_ASSERT_EQUAL_FATAL(frame->sb[4].kind, TYPE(F32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[4].v.f32, 33.232F);

  CU_ASSERT_EQUAL_FATAL(frame->sb[5].kind, TYPE(F64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[5].v.f64, 69.4819392);
  print_call_frame(frame);
}
void test_add(void)
{
  SStack stack;
  SCallFrame *frame = &(stack.frames[0]);
  SFunc func;

  stack.frame = frame;

  frame->sl          = stack.data;
  frame->sb          = stack.data;
  frame->st          = stack.data;
  frame->function    = &func;
  frame->stack       = &stack;
  frame->frame_index = 0;
  frame->ip          = NULL;

  func.name = (SizedStr) {"anonymous", 9};
  func.type = &(SType) { 0, 0, NULL };
  func.code = (U8[]) {
    INSTR(CONST_U32), enc_i32(38199UL),
    INSTR(CONST_U32), enc_i32(2819427UL),
    INSTR(ADD_U32)
  };
  func.code_end = func.code + 11;
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_EQUAL_FATAL(frame->st - frame->sb, 1);
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(U32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.u32, 38199 + 2819427);

  frame->st = frame->sb;
  func.code = (U8[]) {
    INSTR(CONST_S32), enc_i32(-3917L),
    INSTR(CONST_S32), enc_i32(3929L),
    INSTR(ADD_S32)
  };
  func.code_end = func.code + 11;
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(S32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.s32, -3917 + 3929);

  frame->st = frame->sb;
  func.code = (U8[]) {
    INSTR(CONST_U64), enc_i64(38195619ULL),
    INSTR(CONST_U64), enc_i64(199437957LL),
    INSTR(ADD_U64)
  };
  func.code_end = func.code + 19;
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(U64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.u64, 38195619ull + 199437957ull);

  frame->st = frame->sb;
  func.code = (U8[]) {
    INSTR(CONST_S64), enc_i64(-39104782LL),
    INSTR(CONST_S64), enc_i64(39190LL),
    INSTR(ADD_S64)
  };
  func.code_end = func.code + 19;
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(S64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.s64, -39104782ll + 39190ll);

  frame->st = frame->sb;
  func.code = (U8[]) {
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(CONST_F32), 0, 0, 0, 0,
    INSTR(ADD_F32)
  };
  func.code_end = func.code + 11;
  WRITE_F32(func.code + 1, 38.188f);
  WRITE_F32(func.code + 6, 2.434f);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(F32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.f32, 38.188f + 2.434f);

  frame->st = frame->sb;
  func.code = (U8[]) {
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(CONST_F64), 0, 0, 0, 0, 0, 0, 0, 0,
    INSTR(ADD_F64)
  };
  func.code_end = func.code + 19;
  WRITE_F64(func.code + 1, 79.16493854);
  WRITE_F64(func.code + 10, 387.37398);
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
  CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(F64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.f64, 79.16493854 + 387.37398);
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
