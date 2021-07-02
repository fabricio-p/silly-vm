#ifndef SILLY_COMMON_H
#define SILLY_COMMON_H
#include <stdint.h>

enum silly_status {
	SILLY_S_OK,
	SILLY_S_NO_MEMORY,
	SILLY_S_CANT_OPEN_FILE,
	SILLY_S_CANT_WRITE,
	SILLY_S_STACK_OVERFLOW,
	SILLY_S_STACK_EMPTY,
	SILLY_S_STACK_POP_MISMATCH,
	SILLY_S_MEMORY_TO_BIG,
	SILLY_S_MEMORY_UNINITIALIZED,
	SILLY_S_MEMORY_ACCESS_OUT_OF_BOUNDS
};
typedef enum silly_type {
	SILLY_T_I32_U = 1,
	SILLY_T_I32_S,
	SILLY_T_I64_U,
	SILLY_T_I64_S,
	SILLY_T_F32,
	SILLY_T_F64/*,
	SILLY_T_EXTERNREF*/
} silly_type_e;

#define SILLY_MAX_STACK_COUNT 0x100 // 256 stack slots
#define SILLY_MAX_STACK_SIZE							\
	(SILLY_MAX_STACK_COUNT * sizeof(SillyStackSlot))

typedef struct _SillyStackSlot {
	uint8_t		 type;
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
	size_t		   count;
	size_t		   register_count;
	SillyStackSlot *data;
	SillyStackSlot *stack_top;
} SillyStack;

int			   silly_stack_init	   (SillyStack *, size_t);
int			   silly_stack_push	   (SillyStack *, SillyStackSlot *);
int			   silly_stack_pop	   (SillyStack *, int, SillyStackSlot *);
// SillyStack	   silly_stack_split_at(SillyStack *, size_t);
void		   silly_stack_clean   (SillyStack *);

#define SILLY_MEMORY_PAGE_SIZE 0x10000
#define SILLY_MEMORY_MAX_PAGE_COUNT 0x20
#define SILLY_MEMORY_MAX_SIZE								\
	(SILLY_MEMORY_PAGE_SIZE * SILLY_MEMORY_MAX_PAGE_COUNT)

typedef struct _SillyMemory {
	uint8_t page_count;
	size_t	size;
	void	*data;
} SillyMemory;

__attribute__((always_inline))
static inline int silly_memory_size_from_pages(int page_count)
{
	return SILLY_MEMORY_PAGE_SIZE * page_count;
}

int  silly_memory_init (SillyMemory *, uint8_t);
int  silly_memory_write(SillyMemory *, size_t, void *, size_t);
int	 silly_memory_read (SillyMemory *, size_t, void *, size_t);
void silly_memory_clean(SillyMemory *);

#endif /* SILLY_COMMON_H */
