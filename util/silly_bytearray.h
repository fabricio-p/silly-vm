#ifndef SILLY_BYTEARRAY_H
#define SILLY_BYTEARRAY_H
#include "silly_common.h"
#include <stdint.h>

typedef struct _SillyByteArray {
	size_t	length;
	size_t	capacity;
	uint8_t *data;
} SillyByteArray;

int		silly_bytearray_init	  (SillyByteArray *, size_t);
void	silly_bytearray_write_byte(SillyByteArray *, size_t, uint8_t);
void	silly_bytearray_write	  (SillyByteArray *, size_t, void *, size_t);
int		silly_bytearray_append	  (SillyByteArray *, uint8_t);
uint8_t silly_bytearray_read_byte (SillyByteArray *, size_t);
int		silly_bytearray_read	  (SillyByteArray *, size_t, void *, size_t);
int		silly_bytearray_expand	  (SillyByteArray *, size_t);
void	silly_bytearray_clean	  (SillyByteArray *);

#endif /* SILLY_BYTEARRAY_H */
