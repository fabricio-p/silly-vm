#ifndef UTIL_H
#define UTIL_H
#include <inttypes.h>
#include <c-ansi-sequences/graphics.h>
#include "silly.h"

#ifdef ENV_DEV
static void print_value(STaggedValue *val)
{
#ifdef OUTCOLORS
#define SET_TYPE_FG() printf(ANSISEQ_SETFG256(190))
#define RESET_TYPE_FG() printf(ANSISEQ_GR_RESET)
#else
#define SET_TYPE_FG()
#define RESET_TYPE_FG()
#endif /* OUTCOLORS */
  SET_TYPE_FG();
  switch (val->kind)
  {
#define print_case(type, fmt, ...)      \
  case TYPE(type):                      \
    printf("%" fmt, __VA_ARGS__); break

    print_case(I32, PRIi32 " | %" PRIu32, val->v.s32, val->v.u32);
    print_case(I64, PRIi64 " | %" PRIu64, val->v.s64, val->v.u64);
    print_case(F32, ".6f",                val->v.f32);
    print_case(F64, ".12lF",              val->v.f64);
  default:
    printf("<\?\?>");
#undef print_case
  }
  RESET_TYPE_FG();
#undef SET_TYPE_FG
#undef RESET_TYPE_FG
}
static void print_call_frame(SCallFrame *frame)
{
#ifdef OUTCOLORS
#define NAME_FG ANSISEQ_SETFG_CYAN
#define GR_RESET ANSISEQ_GR_RESET
#else
#define NAME_FG
#define GR_RESET
#endif /* OUTCOLORS */
  printf("============ Stack Frame ("NAME_FG);
  fwrite(frame->function->name.str, frame->function->name.len,
         1, stdout);
  puts(GR_RESET") ============");
  for (STaggedValue *val = frame->st - 1,
                    *beg = frame->sb; val >= beg; --val)
  {
    printf("-%*s[%d] %s: ", (int)frame->function->name.len + 3, "",
           val - beg, type_name(val->kind));
    print_value(val);
    puts("");
  }
  printf("========================================");
  for (Usize i = 0; i < frame->function->name.len; ++i)
  {
    putchar('=');
  }
  putchar('\n');
}
#else
#define print_call_frame(frame)
#define print_value(val)
#endif /* ENV_DEV */

#endif /* UTIL_H */
