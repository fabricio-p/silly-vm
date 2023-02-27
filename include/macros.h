#ifndef MACROS_H
#define MACROS_H
#include <c-ansi-sequences/graphics.h>

#define __inline__ static inline __attribute__((always_inline))

#ifdef ENV_DEV
#ifdef OUTCOLORS
#define LOG_DEBUG(...) {              \
  printf(ANSIQ_GR_SEQ(                \
          ANSIQ_FG256(12)             \
          ANSIQ_TXT_BLD               \
         )                            \
         "[DEBUG("__FILE__", %d)]:"   \
         ANSIQ_GR_RESET" ",           \
         __LINE__);                   \
  printf(__VA_ARGS__);                \
  printf("\n");                       \
}
#else
#define LOG_DEBUG(...) {              \
  printf("[DEBUG("__FILE__", %d)]: ", \
         __LINE__);                   \
  printf(__VA_ARGS__);                \
  printf("\n");                       \
}
#endif /* OUTCOLORS */
#else
#define LOG_DEBUG(...)
#endif /* ENV_DEV */

#define LOG_EVAL(expr, fmt) {         \
  LOG_DEBUG(#expr " = %" fmt, expr);  \
}

#ifdef OUTCOLORS
#define LOG_ERROR(...) {                      \
  fprintf(stderr, ANSIQ_GR_SEQ(               \
                    ANSIQ_FG256(9)            \
                    ANSIQ_TXT_BLD             \
                  )                           \
                  "[ERROR("__FILE__", %d)]:"  \
                  ANSIQ_GR_RESET" "           \
                  ANSIQ_SETBG256(52),         \
                  __LINE__);                  \
  fprintf(stderr, __VA_ARGS__);               \
  fprintf(stderr, ANSIQ_GR_RESET"\n");        \
}
#else
#define LOG_ERROR(...) {                        \
  fprintf(stderr, "[ERROR("__FILE__", %d)]: ",  \
                  __LINE__);                    \
  fprintf(stderr, __VA_ARGS__);                 \
  fprintf(stderr, ANSIQ_GR_RESET"\n");          \
}
#endif /* OUTCOLORS */

#define INSTR(instr) SILLY_INSTR_##instr
#define TYPE(type) SILLY_TYPE_##type

#endif /* MACROS_H */
