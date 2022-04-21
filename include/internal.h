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

#define STORE_U8(ptr, val) WRITE_U8(ptr, val)
#define STORE_S8(ptr, val) WRITE_S8(ptr, val)
#define STORE_F32(ptr, val) WRITE_F32(ptr, val)
#define STORE_F64(ptr, val) WRITE_F64(ptr, val)
#define LOAD_U8(ptr) (*(U8 *)(ptr))
#define LOAD_S8(ptr) (*(S8 *)(ptr))
#define LOAD_F32(ptr, val) READ_F32(ptr, val)
#define LOAD_F64(ptr, val) READ_F64(ptr, val)

#if SILLY_LITTLE_ENDIAN != 0
#define STORE_U16(ptr, val) WRITE_U16(ptr, val)
#define STORE_U32(ptr, val) WRITE_U32(ptr, val)
#define STORE_U64(ptr, val) WRITE_U64(ptr, val)
#define STORE_S16(ptr, val) WRITE_S16(ptr, val)
#define STORE_S32(ptr, val) WRITE_S32(ptr, val)
#define STORE_S64(ptr, val) WRITE_S64(ptr, val)

#define LOAD_U16(ptr) READ_U16(ptr)
#define LOAD_U32(ptr) READ_U32(ptr)
#define LOAD_U64(ptr) READ_U64(ptr)
#define LOAD_S16(ptr) READ_S16(ptr)
#define LOAD_S32(ptr) READ_S32(ptr)
#define LOAD_S64(ptr) READ_S64(ptr)
#else
#define STORE_U16(ptr, val)             \
  do {                                  \
    union {                             \
      U16 u16;                          \
      U8  u8[2];                        \
    } v = { .u16 = (val) };             \
    U8 *u8_ptr = (ptr);                 \
    u8_ptr[0] = v.u8[1];                \
    u8_ptr[1] = v.u8[0];                \
  } while (0);
#define STORE_U32(ptr, val)             \
  do {                                  \
    union {                             \
      U32 u32;                          \
      U16 u16[2];                       \
    } v = { .u32 = (val) };             \
    U16 *u16_ptr = (ptr);               \
    STORE_U16(&(u16_ptr[0]), v.u16[1]); \
    STORE_U16(&(u16_ptr[1]), v.u16[0]); \
  } while (0);
#define STORE_U64(ptr, val)             \
  do {                                  \
    union {                             \
      U64 u64;                          \
      U32 u32[2];                       \
    } v = { .u64 = (val) };             \
    U32 *u32_ptr = (ptr);               \
    STORE_U32(&(u32_ptr[0]), v.u32[1]); \
    STORE_U32(&(u32_ptr[1]), v.u32[0]); \
  } while (0);
#define STORE_S16(ptr, val) STORE_U16(ptr, (U16)(val))
#define STORE_S32(ptr, val) STORE_U32(ptr, (U32)(val))
#define STORE_S64(ptr, val) STORE_U64(ptr, (U64)(val))

__inline__
U16 LOAD_U16(void *ptr)
{
  union {
    U8  u8[2];
    U16 u16;
  } v = { .u8[1] = READ_U8(ptr), .u8[0] = READ_U8(ptr + 1) };
  return v.u16;
}
__inline__
U32 LOAD_U32(void *ptr)
{
  union {
    U16 u16[2];
    U32 u32;
  } v = {
    .u16[1] = LOAD_U16(ptr),
    .u16[0] = LOAD_U16(ptr + sizeof(U16))
  };
  return v.u32;
}
__inline__
U64 LOAD_U64(void *ptr)
{
  union {
    U32 u32[2];
    U64 u64;
  } v = {
    .u32[1] = LOAD_U32(ptr),
    .u32[0] = LOAD_U32(ptr + sizeof(U32))
  };
  return v.u64;
}
#define LOAD_S16(ptr) ((S16)LOAD_U16(ptr))
#define LOAD_S32(ptr) ((S32)LOAD_U32(ptr))
#define LOAD_S64(ptr) ((S64)LOAD_U64(ptr))
#endif /* SILLY_LITTLE_ENDIAN != 0 */

#define CVECTOR_POINTERMODE
#include <c-vector/lib.h> // CVECTOR_WITH_NAME, CVECTOR
CVECTOR_WITH_NAME(SType, STypeVec);
CVECTOR_WITH_NAME(STaggedValue,  STaggedValueVec);
CVECTOR_WITH_NAME(SFunc, SFuncVec);
CVECTOR(U8);

#endif /* INTERNAL_H */
