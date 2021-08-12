#ifndef SILLY_COMMON_H
#define SILLY_COMMON_H
#include <stdint.h>
#define __silly_inline__						\
	__attribute__((always_inline)) static inline

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
	SILLY_S_MEMORY_ACCESS_OUT_OF_BOUNDS,
	SILLY_S_UNKNOWN_NUMBER_TYPE
};
/* typedef */ enum silly_stack_type {
	SILLY_T_I32_U = 1,
	SILLY_T_I32_S,
	SILLY_T_I64_U,
	SILLY_T_I64_S,
	SILLY_T_F32,
	SILLY_T_F64/*,
	SILLY_T_EXTERNREF*/
};
typedef uint8_t silly_stack_type;

enum silly_memory_type {
	SILLY_MT_I8_U  = 0b000100,
	SILLY_MT_I8_S  = SILLY_MT_I8_U | 1,
	SILLY_MT_I16_U = 0b001000,
	SILLY_MT_I16_S = SILLY_MT_I16_U | 1,
	SILLY_MT_I32_U = 0b010000,
	SILLY_MT_I32_S = SILLY_MT_I32_U | 1,
	SILLY_MT_F32   = SILLY_MT_I32_U | 2,
	SILLY_MT_I64_U = 0b100000,
	SILLY_MT_I64_S = SILLY_MT_I64_U | 1,
	SILLY_MT_F64   = SILLY_MT_I64_U | 2
};
typedef uint8_t silly_memory_type;


typedef struct _SillyType {
	uint16_t param_count;
	uint16_t result_count;
	uint8_t  types[1];
} SillyType;

typedef struct _SillyFunc {
	void      **ip;
	void      **body;
	SillyType *type;
	size_t    size;
	int       (*native)(void *);
} SillyFunc;

#define SILLY_MAX_STACK_COUNT 0x100 // 256 stack slots
#define SILLY_MAX_STACK_SIZE							\
	(SILLY_MAX_STACK_COUNT * sizeof(SillyStackSlot))

typedef union _SillyStackValue {
	uint32_t i32_u;
	int32_t  i32_s;
	uint64_t i64_u;
	int64_t	 i64_s;
	float    f32;
	double   f64;
} SillyStackValue;

typedef struct _SillyStackSlot {
	uint8_t         type;
	SillyStackValue value;
	// We can use the padding made by the compiler later
	uint8_t         extra[7];
} SillyStackSlot;

__silly_inline__
void silly_stackslot_init(SillyStackSlot *slot, uint8_t type,
							SillyStackValue value)
{
	slot->type = type;
	slot->value = value;
}

typedef struct _SillyStack {
	size_t             size;
	size_t             count;
	size_t             slot_count;
	size_t             register_count;
	struct {
		char   *name;
		size_t name_length;
	}				   *symbol;
	struct _SillyStack *parent;
	struct _SillyStack *next;
	SillyStackSlot     *data;
	SillyStackSlot     *data_start;
	SillyStackSlot     *stack_top;
} SillyStack;

int			   silly_stack_init	(SillyStack *, size_t, size_t, void *);
int			   silly_stack_push	(SillyStack *, SillyStackSlot *);
int			   silly_stack_pop	(SillyStack *, int, SillyStackSlot *);
__silly_inline__
void		   silly_stack_clear(SillyStack *stack)
{
	stack->stack_top = stack->data_start;
	stack->count = 0;
}
int			   silly_stack_drop	(SillyStack *);
SillyStack	   silly_stack_split(SillyStack *, SillyType *, void *, size_t);
__silly_inline__
void		   silly_stack_link	(SillyStack *stack, SillyStack *other)
{
	stack->next = other;
	other->parent = stack;
}
void		   silly_stack_dump	(SillyStack *);
void		   silly_stack_clean(SillyStack *);

#define SILLY_MEMORY_PAGE_SIZE 0x10000
#define SILLY_MEMORY_MAX_PAGE_COUNT 0x20
#define SILLY_MEMORY_MAX_SIZE								\
	(SILLY_MEMORY_PAGE_SIZE * SILLY_MEMORY_MAX_PAGE_COUNT)

typedef struct _SillyMemory {
	uint8_t page_count;
	size_t	size;
	void	*data;
} SillyMemory;

__silly_inline__
int silly_memory_size_from_pages(int page_count)
{
	return SILLY_MEMORY_PAGE_SIZE * page_count;
}

int  silly_memory_init (SillyMemory *, uint8_t);
int  silly_memory_write(SillyMemory *, size_t, void *, size_t);
int	 silly_memory_read (SillyMemory *, size_t, void *, size_t);
int	 silly_memory_store(SillyMemory *, size_t, silly_memory_type,
						SillyStackValue);
int  silly_memory_load (SillyMemory *, size_t, silly_memory_type,
						SillyStackSlot *);
void silly_memory_clean(SillyMemory *);
int silly_memory_sizeof(silly_memory_type);

typedef struct _SillyContext {
	SillyStack	*stack;
	SillyMemory	*memory;
	SillyFunc	*func;
} SillyContext;

#endif /* SILLY_COMMON_H */
