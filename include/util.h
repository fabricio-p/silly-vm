#ifndef UTIL_H
#define UTIL_H
#include "silly.h"

#ifdef ENV_DEV
static void print_value(SValue *val)
{
  switch (val->kind)
  {
#define print_case(type, fmt, field)        \
  case TYPE(type):                          \
    printf(fmt, val->field); break

    print_case(U32, "%lu",   u32);
    print_case(S32, "%li",   s32);
    print_case(U64, "%llu",  u64);
    print_case(S64, "%lli",  s64);
    print_case(F32, "%.6f",  f32);
    print_case(F64, "%.12F", f64);
  default:
    puts("unknown type");
#undef print_case
  }
}
static void print_call_frame(SCallFrame *frame)
{
  printf("============ Stack Frame (");
  fwrite(frame->function->name->str, frame->function->name->len,
         1, stdout);
  puts(") ============");
  for (SValue *val = frame->st - 1, *beg = frame->sb; val >= beg; --val)
  {
    printf("-%*s[%d] %s: ", (int)frame->function->name->len + 3, "",
           val - beg, type_name(val->kind));
    print_value(val);
    puts("");
  }
  printf("========================================");
  for (Usize i = 0; i < frame->function->name->len; ++i)
  {
    putc('=', stdout);
  }
  putc('\n', stdout);
}
#else
#define print_call_frame(frame)
#define print_value(val)
#endif /* ENV_DEV */

#endif /* UTIL_H */
