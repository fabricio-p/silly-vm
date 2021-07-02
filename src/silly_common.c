#include "silly_common.h"
#include <malloc.h>

int silly_stack_init(SillyStack *stack, size_t count)
{
	stack->size = count * sizeof(SillyStackSlot);
	stack->count = count;
	stack->register_count = 0;
	stack->data = (SillyStackSlot *)calloc(count, sizeof(SillyStackSlot));
	if (stack->data == NULL)
		return SILLY_S_NO_MEMORY;
	stack->stack_top = stack->data;
	return SILLY_S_OK;
}
int silly_stack_push(SillyStack *stack, SillyStackSlot *value)
{
	if (stack->count == SILLY_MAX_STACK_COUNT)
	{
		if (stack->count == SILLY_MAX_STACK_COUNT)
			return SILLY_S_STACK_OVERFLOW;
		else
		{
			stack->data = (SillyStackSlot *)realloc(stack->data,
													SILLY_MAX_STACK_SIZE);
			if (stack->data == NULL)
				return SILLY_S_NO_MEMORY;
		}
	}
	memcpy(stack->stack_top, value, sizeof(SillyStackSlot));
	stack->stack_top = &stack->stack_top[1];
	stack->count++;
	return SILLY_S_OK;
}
int silly_stack_pop(SillyStack *stack, int type,
							   SillyStackSlot *target)
{
	if (stack->count == 0)
		return SILLY_S_STACK_EMPTY;
	else
	{
		if (type != stack->stack_top[-1].type)
			return SILLY_S_STACK_POP_MISMATCH;
		memcpy(target, (stack->stack_top = &stack->stack_top[-1]),
				sizeof(SillyStackSlot));
		stack->count--;
		return SILLY_S_OK;
	}
}
void silly_stack_clean(SillyStack *stack)
{
	if (stack != NULL)
	{
		if (stack->data != NULL)
			free(stack->data);
		stack->data =
			stack->stack_top = NULL;
		stack->register_count = 0;
		stack->size = 0;
		stack->count = 0;
	}
}
int silly_memory_init(SillyMemory *memory, uint8_t page_count)
{
	if (page_count != 0)
	{
		memory->size = silly_memory_size_from_pages(page_count);
		memory->data = malloc(memory->size);
		if (memory->data == NULL)
			return SILLY_S_NO_MEMORY;
		
	}
	else
		memory->data = NULL;
	memory->page_count = page_count;
	return SILLY_S_OK;
}
int silly_memory_write(SillyMemory *memory, size_t offset, void *source,
						size_t size)
{
	if (memory->data == NULL)
		return SILLY_S_MEMORY_UNINITIALIZED;
	if (offset >= memory->size)
		return SILLY_S_MEMORY_ACCESS_OUT_OF_BOUNDS;
	if (size + offset >= memory->size)
		return SILLY_S_MEMORY_TO_BIG;
	memcpy(memory->data + offset, source, size);
	return SILLY_S_OK;
}
int silly_memory_read(SillyMemory *memory, size_t offset, void *dest,
						size_t size)
{
	if (memory->data == NULL)
		return SILLY_S_MEMORY_UNINITIALIZED;
	if (offset >= memory->size)
		return SILLY_S_MEMORY_ACCESS_OUT_OF_BOUNDS;
	if (size + offset >= memory->size)
		return SILLY_S_MEMORY_TO_BIG;
	memcpy(dest, memory->data + offset, size);
	return SILLY_S_OK;
}
void silly_memory_clean(SillyMemory *memory)
{
	if (memory != NULL)
	{
		if (memory->data != NULL)
			free(memory->data);
		memory->data = NULL;
		memory->page_count = 0;
		memory->size = 0;
	}
}
