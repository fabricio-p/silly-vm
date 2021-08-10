#include "silly_vector.h"
#include "silly_common.h"
#include <malloc.h>
#include <string.h>
#define ALIGN(n) (((n) + 8 - 1) & ~7)

void silly_vector_init(SillyVector *vec, size_t item_size)
{
	vec->item_size = item_size;
	vec->length = vec->capacity = vec->size = 0;
	vec->data = NULL;
}
int silly_vector_with_capacity(SillyVector *vec, size_t item_size,
								size_t capacity)
{
	silly_vector_init(vec, item_size);
	vec->capacity = capacity;
	vec->size = ALIGN(capacity * item_size);
	if ((vec->data = malloc(vec->size)) == NULL)
		return SILLY_S_NO_MEMORY;
	memset(vec->data, 0, vec->size);
	return SILLY_S_OK;
}
int silly_vector_resize(SillyVector *vec, size_t new_capacity)
{
	vec->size = ALIGN(new_capacity * vec->item_size);
	void *new_data = realloc(vec->data, vec->size);
	if (new_data == NULL)
	{
		silly_vector_clean(vec);
		return SILLY_S_NO_MEMORY;
	}
	vec->capacity = new_capacity;
	vec->data = new_data;
	return SILLY_S_OK;
}
__silly_inline__
void split_half(SillyVector *vec)
{
	silly_vector_resize(vec, vec->capacity >> 1);
}
void *silly_vector_at(SillyVector *vec, size_t index)
{
	if (vec->data == NULL || index >= vec->length)
		return NULL;
	return silly_vector_get(vec, index);
}
void silly_vector_pop(SillyVector *vec, void *dest)
{
	if (vec->length == 0)
		return;
	memcpy(dest, vec->data + --(vec->length), vec->item_size);
	if (vec->length <= vec->capacity >> 2)
		split_half(vec);
}
void silly_vector_push(SillyVector *vec, void *src)
{
	if (vec->length >= vec->capacity)
		if (silly_vector_resize(vec, (int)(vec->capacity / 2 * 3)))
			return;
	memcpy(vec->data + (vec->length)++ * vec->item_size, src, vec->item_size);
}
void silly_vector_clear(SillyVector *vec)
{
	size_t item_size = vec->item_size;
	silly_vector_clean(vec);
	silly_vector_init(vec, item_size);
}
void silly_vector_clean(SillyVector *vec)
{
	if (vec != NULL)
	{
		if (vec->data != NULL)
			free(vec->data);
		vec->data = NULL;
		vec->item_size = vec->length = vec->capacity = vec->size = 0;
	}
}
