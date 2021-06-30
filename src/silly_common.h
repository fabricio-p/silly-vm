#ifndef SILLY_COMMON_H
#define SILLY_COMMON_H
#include <stdint.h>

typedef enum silly_type {
	SILLY_T_I32_U = 1,
	SILLY_T_I32_S,
	SILLY_T_I64_U,
	SILLY_T_I64_S,
	SILLY_T_F32,
	SILLY_T_F64/*,
	SILLY_T_EXTERNREF*/
} silly_type_e;

typedef struct _SillyStackSlot {
	silly_type_e type;
	union {
		uint32_t i32_u;
		int32_t	 i32_s;
		uint64_t i64_u;
		int64_t	 i64_s;
		float	 f32;
		double	 f64;
	}			 value;
} SillyStackSlot;

typedef struct _SillyStack {
	size_t		   size;
	size_t		   max_size;
	size_t		   count;
	size_t		   register_count;
	SillyStackSlot *data;
	SillyStackSlot *stack_top;
} SillyStack;

void		   silly_stack_init	   (SillyStack *, size_t, size_t, size_t);
void		   silly_stack_push	   (SillyStack *, SillyStackSlot);
SillyStackSlot silly_stack_pop	   (SillyStack *, soap_type_e);
SillyStack	   silly_stack_split_at(SillyStack *, size_t);
void		   silly_stack_clean   (SillyStack *);

typedef struct _SillyMemory {
	size_t page_count;
	size_t max_pages;
	void   *data;
} SillyMemory;

void silly_memory_init (SillyMemory *, size_t, size_t);
void silly_memory_write(SillyMemory *, void *, size_t);
void silly_memory_read (SillyMemory *, void *, size_t);
void silly_memory_clean(SillyMemory *);

#endif /* SILLY_COMMON_H */
