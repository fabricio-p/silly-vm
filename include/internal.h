#ifndef INTERNAL_H
#define INTERNAL_H
#include "silly.h"
#include "macros.h"
#include "config.h"

#define WRITE_U32(ptr, val) (*((U32 *)(ptr))) = (val)
#define WRITE_S32(ptr, val) (*((S32 *)(ptr))) = (val)
#define WRITE_U16(ptr, val) (*((U16 *)(ptr))) = (val)
#define WRITE_S16(ptr, val) (*((S16 *)(ptr))) = (val)

#define READ_U32(ptr) (*((U32 *)(ptr)))
#define READ_S32(ptr) (*((S32 *)(ptr)))
#define READ_U16(ptr) (*((U16 *)(ptr)))
#define READ_S16(ptr) (*((S16 *)(ptr)))

#if SILLY_CPU_UNALIGNED_POINTERS != 0

#define WRITE_U64(ptr, val) *(((U64 *)(ptr))) = (val)
#define WRITE_S64(ptr, val) *(((S64 *)(ptr))) = (val)
#define WRITE_F32(ptr, val) *(((F32 *)(ptr))) = (val)
#define WRITE_F64(ptr, val) *(((F64 *)(ptr))) = (val)

#define READ_U64(ptr) (*((U64 *)(ptr)))
#define READ_S64(ptr) (*((S64 *)(ptr)))
#define READ_F32(ptr) (*((F32 *)(ptr)))
#define READ_F64(ptr) (*((F64 *)(ptr)))

#else

#define WRITE_U64(ptr, val)       \
  do {                            \
    union {                       \
      U32 parts[2];               \
      U64 value;                  \
    } v;                          \
    v.value = (val);              \
    U32 *u32_ptr = (U32 *)(ptr);  \
    u32_ptr[0] = v.parts[0];      \
    u32_ptr[1] = v.parts[1];      \
  } while (0);

#define WRITE_S64(ptr, val) WRITE_U64(ptr, (U64)(val))

#define WRITE_F32(ptr, val)       \
  do {                            \
    union {                       \
      F32 f32;                    \
      U32 u32;                    \
    } v;                          \
    v.f32 = (val);                \
    U32 *u32_ptr = (U32 *)(ptr);  \
    u32_ptr[0] = v.u32;           \
  } while (0);

#define WRITE_F64(ptr, val)       \
  do {                            \
    union {                       \
      F64 value;                  \
      U32 parts[2];               \
    } v;                          \
    v.value = (val);              \
    U32 *u32_ptr = (U32 *)(ptr);  \
    u32_ptr[0] = v.parts[0];      \
    u32_ptr[1] = v.parts[1];      \
  } while (0);

__inline__
U64 READ_U64(void const *const ptr)
{
  union {
    U64 value;
    U32 parts[2];
  } v;
  U32 const *const u32_ptr = ptr;
  v.parts[0] = u32_ptr[0];
  v.parts[1] = u32_ptr[1];
  return v.value;
}
#define READ_S64(ptr) ((S64)READ_U64(ptr))
__inline__
F32 READ_F32(void const *const ptr)
{
  union {
    F32 value;
    U32 u32;
  } v;
  U32 const *const u32_ptr = ptr;
  v.u32 = u32_ptr[0];
  return v.value;
}
__inline__
F64 READ_F64(void const *const ptr)
{
  union {
    F64 value;
    U64 u64;
  } v;
  v.u64 = READ_U64(ptr);
  return v.value;
}

#endif /* SILLY_CPU_UNALIGNED_ADDRESSES != 0 */

#define CVECTOR_POINTERMODE
#include <c-vector/lib.h> // CVECTOR_WITH_NAME, CVECTOR

CVECTOR_WITH_NAME(SType *, STypeVec);
CVECTOR_WITH_NAME(SValue,  SValueVec);
CVECTOR_WITH_NAME(SFunc *, SFuncVec);

#endif /* INTERNAL_H */
