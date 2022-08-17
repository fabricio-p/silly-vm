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

typedef U32 I32;
typedef U64 I64;
#define POOL(k, ...)                              \
[(SILLY_TYPE_##k >> 2) - 1] = {                   \
  .data = (k[]) {__VA_ARGS__},                    \
  .size = sizeof((k[]) {__VA_ARGS__}) / sizeof(k) \
}

// TODO: Implement tests for every opcode
// TODO: Make macros so it is easier to test operations on every type

void test_const(void)
{
  SStack stack;
  SCallFrame *frame = &(stack.frames[0]);
  SFunc func;

  stack.frame       = frame;

  frame->sl          = stack.data;
  frame->sb          = stack.data;
  frame->st          = stack.data;
  frame->function    = &func;
  frame->stack       = &stack;
  frame->frame_index = 0;
  frame->ip          = NULL;
#define strify(expr) #expr

  func.type = &(SType) { 0, 0, NULL };
  func.code = (U8[]) {
    INSTR(CONST_I32), enc_i32(0),
    INSTR(CONST_I32), enc_i32(1),
    INSTR(CONST_I64), enc_i32(0),
    INSTR(CONST_I64), enc_i32(1),
    INSTR(CONST_F32), enc_i32(0),
    INSTR(CONST_F64), enc_i32(0)
  };
  SCPool pools[] = {
    POOL(I32, 325432UL, -29191L),
    POOL(I64, 4928277823ULL, -1849183LL),
    POOL(F32, 33.232F),
    POOL(F64, 69.4819392)
  };
  func.code_end = func.code + 30;
  func.name = (SizedStr) {"foo", 3};
  CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame, pools), SILLY_E_OK);

  CU_ASSERT_EQUAL_FATAL(frame->st - frame->sb, 6);

  CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(I32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.u32, 325432UL);

  CU_ASSERT_EQUAL_FATAL(frame->sb[1].kind, TYPE(I32));
  CU_ASSERT_EQUAL_FATAL(frame->sb[1].v.s32, -29191L);

  CU_ASSERT_EQUAL_FATAL(frame->sb[2].kind, TYPE(I64));
  CU_ASSERT_EQUAL_FATAL(frame->sb[2].v.u64, 4928277823ULL);

  CU_ASSERT_EQUAL_FATAL(frame->sb[3].kind, TYPE(I64));
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
  do {
    func.code = (U8[]) {
      INSTR(CONST_I32), enc_i32(0),
      INSTR(CONST_I32), enc_i32(1),
      INSTR(ADD_I32)
    };
    SCPool pools[] = {
      POOL(I32, -3917L, 3929L),
    };
    func.code_end = func.code + 11;
    CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame, pools), SILLY_E_OK);

    CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
    CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(I32));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.s32, -3917 + 3929);
  } while (0);

  do {
    frame->st = frame->sb;
    SCPool pools[] = {
      POOL(I64, -39104782LL, 39190LL),
    };
    func.code = (U8[]) {
      INSTR(CONST_I64), enc_i32(0),
      INSTR(CONST_I64), enc_i32(1),
      INSTR(ADD_I64)
    };
    func.code_end = func.code + 11;
    CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame, pools), SILLY_E_OK);

    CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
    CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(I64));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.s64, -39104782ll + 39190ll);
  } while (0);

  do {
    frame->st = frame->sb;
    func.code = (U8[]) {
      INSTR(CONST_F32), enc_i32(0),
      INSTR(CONST_F32), enc_i32(1),
      INSTR(ADD_F32)
    };
    func.code_end = func.code + 11;
    SCPool pools[] = {
      POOL(F32, 38.188f, 2.434f),
    };
    CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame, pools), SILLY_E_OK);

    CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
    CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(F32));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.f32, 38.188f + 2.434f);
  } while (0);

  do {
    frame->st = frame->sb;
    func.code = (U8[]) {
      INSTR(CONST_F64), enc_i32(0),
      INSTR(CONST_F64), enc_i32(1),
      INSTR(ADD_F64)
    };
    func.code_end = func.code + 11;
    SCPool pools[] = {
      POOL(F64, 79.16493854, 387.37398)
    };
    CU_ASSERT_EQUAL_FATAL(SFunc_exec(NULL, frame, pools), SILLY_E_OK);

    CU_ASSERT_EQUAL_FATAL(frame->ip, func.code_end);
    CU_ASSERT_PTR_EQUAL_FATAL(frame->st, &(frame->sb[1]));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].kind, TYPE(F64));
    CU_ASSERT_EQUAL_FATAL(frame->sb[0].v.f64, 79.16493854 + 387.37398);
  } while (0);
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
