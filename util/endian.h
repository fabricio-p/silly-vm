#ifndef SILLY_UTIL_ENDIAN_H
#define SILLY_UTIL_ENDIAN_H
#include "silly_common.h"

__silly_inline__
void write_i8_u(void *ptr, uint8_t value)
{
	*(uint8_t *)ptr = value;
}
__silly_inline__
void write_i8_s(void *ptr, int8_t value)
{
	*(int8_t *)ptr = value;
}
__silly_inline__
uint8_t read_i8_u(void *ptr)
{
	return *(uint8_t *)ptr;
}
__silly_inline__
int8_t read_i8_s(void *ptr)
{
	return *(int8_t *)ptr;
}
__silly_inline__
void write_f32(void *ptr, float value)
{
	*((float *)ptr) = value;
}
__silly_inline__
void write_f64(void *ptr, double value)
{
	*((double *)ptr) = value;
}
__silly_inline__
float read_f32(void *ptr)
{
	return *((float *)ptr);
}
__silly_inline__
double read_f64(void *ptr)
{
	return *((double *)ptr);
}
#ifndef __LITTLE_ENDIAN__
__silly_inline__
void write_i16_u(void *ptr, uint16_t value)
{
	write_i8_u(ptr	  , (uint8_t)value &  0xff);
	write_i8_u(ptr + 1, (uint8_t)value >> 8);
}
__silly_inline__
void write_i16_s(void *ptr, int16_t value)
{
	write_i16_u(ptr, (uint16_t)value);
}
__silly_inline__
void write_i32_u(void *ptr, uint32_t value)
{
	write_i16_u(ptr	   , (uint16_t)value & 0xffff);
	write_i16_u(ptr + 2, (uint16_t)value >> 16);
}
__silly_inline__
void write_i32_s(void *ptr, int32_t value)
{
	write_i32_u(ptr, (uint32_t)value);
}
__silly_inline__
void write_i64_u(void *ptr, uint64_t value)
{
	write_i32_u(ptr	   , (uint32_t)(value &  0xffffffff));
	write_i32_u(ptr + 4, (uint32_t)(value >> 32));
}
__silly_inline__
void write_i64_s(void *ptr, int64_t value)
{
	write_i64_u(ptr, (uint64_t)value);
}
__silly_inline__
uint16_t read_i16_u(void *ptr)
{
	return (
			((uint16_t)read_i8_u(ptr)) |
			((uint16_t)read_i8_u(ptr + 1) << 8)
		   );
}
__silly_inline__
int16_t read_i16_s(void *ptr)
{
	return (int16_t)read_i16_u(ptr);
}
__silly_inline__
uint32_t read_i32_u(void *ptr)
{
	return (
			((uint32_t)read_i16_u(ptr)) |
			((uint32_t)read_i16_u(ptr + 2) << 16)
		   );
}
__silly_inline__
int32_t read_i32_s(void *ptr)
{
	return (int32_t)read_i32_u(ptr);
}
__silly_inline__
uint64_t read_i64_u(void *ptr)
{
	return (
			((uint64_t)read_i32_u(ptr)) |
			((uint64_t)read_i32_u(ptr + 4) << 32)
		   );
}
__silly_inline__
int64_t read_i64_s(void *ptr)
{
	return (int64_t)read_i64_u(ptr);
}
#else
__silly_inline__
void write_i16_u(void *ptr, uint16_t value)
{
	*(uint16_t *)ptr = value;
}
__silly_inline__
void write_i16_s(void *ptr, int16_t value)
{
	*(int16_t *)ptr = value;
}
__silly_inline__
void write_i32_u(void *ptr, uint32_t value)
{
	*(uint32_t *)ptr = value;
}
__silly_inline__
void write_i32_s(void *ptr, int32_t value)
{
	*(int32_t *)ptr = value;
}
__silly_inline__
void write_i64_u(void *ptr, uint64_t value)
{
	*(uint64_t *)ptr = value;
}
__silly_inline__
void write_i64_s(void *ptr, int64_t value)
{
	*(uint64_t *)ptr = value;
}

__silly_inline__
uint16_t read_i16_u(void *ptr)
{
	return *(uint16_t *)ptr;
}
__silly_inline__
int16_t read_i16_s(void *ptr)
{
	return *(int16_t *)ptr;
}
__silly_inline__
uint32_t read_i32_u(void *ptr)
{
	return *(uint32_t *)ptr;
}
__silly_inline__
int32_t read_i32_s(void *ptr)
{
	return *(int32_t *)ptr;
}
__silly_inline__
uint64_t read_i64_u(void *ptr)
{
	return *(uint64_t *)ptr;
}
__silly_inline__
int64_t read_i64_s(void *ptr)
{
	return *(int64_t *)ptr;
}
#endif /* __LITTLE_ENDIAN__ */
#endif /* SILLY_UTIL_ENDIAN_H */
