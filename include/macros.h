#ifndef MACROS_H
#define MACROS_H

#define __inline__ static inline __attribute__((always_inline))

#define LOG_DEBUG(...) {              \
  printf("[DEBUG(%d)]: ", __LINE__);  \
  printf(__VA_ARGS__);                \
  printf("\n");                       \
}
#define LOG_EVAL(expr, fmt) {         \
  LOG_DEBUG(#expr " = %" fmt, expr);\
}

#endif /* MACROS_H */
