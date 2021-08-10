#ifndef SILLY_VECTOR_H
#define SILLY_VECTOR_H
#include <stdint.h>
#include <stddef.h>

typedef struct _SillyVector {
	size_t item_size;
	size_t length;
	size_t capacity;
	size_t size;
	void   *data;
} SillyVector;

void silly_vector_init		   (SillyVector *, size_t);
int	 silly_vector_with_capacity(SillyVector *, size_t, size_t);
void *silly_vector_at		   (SillyVector *, size_t);
void silly_vector_pop		   (SillyVector *, void *);
void silly_vector_push		   (SillyVector *, void *);
__attribute__((always_inline)) static inline
void *silly_vector_get		   (SillyVector *vec, size_t index)
{
	return vec->data + index * vec->item_size;
}
void silly_vector_clear		   (SillyVector *);
void silly_vector_clean		   (SillyVector *);

#endif /* SILLY_VECTOR_H */
