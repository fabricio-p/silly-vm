#ifndef MACROS_H
#define MACROS_H

#define __inline__ static inline __attribute__((always_inline))

#ifdef ENV_DEV
#define LOG_DEBUG(...) {              \
  printf("[DEBUG("__FILE__", %d)]: ", \
         __LINE__);                   \
  printf(__VA_ARGS__);                \
  printf("\n");                       \
}
#define LOG_EVAL(expr, fmt) {         \
  LOG_DEBUG(#expr " = %" fmt, expr);  \
}
#else
#define LOG_DEBUG(...)
#define LOG_EVAL(expr, fmt)
#endif /* ENV_DEV */

#define LOG_ERROR(...) {              \
  fprintf(stderr, "[ERROR]: ");       \
  fprintf(stderr, __VA_ARGS__);       \
}

#define INSTR(instr) SILLY_INSTR_##instr
#define TYPE(type) SILLY_TYPE_##type

#endif /* MACROS_H */
