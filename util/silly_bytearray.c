#include "silly_bytearray.h"
#include <malloc.h>

int silly_bytearray_init(SillyByteArray *bytearray, size_t capacity)
{
	bytearray->capacity = capacity;
	if (capacity != 0)
		if((bytearray->data = (uint8_t *)malloc(capacity)) == NULL)
		{
			silly_bytearray_clean(bytearray);
			return SILLY_S_NO_MEMORY;
		}
	return SILLY_S_OK;
}
void silly_bytearray_write_byte(SillyByteArray *bytearray, size_t offset,
								uint8_t data)
{
	if (offset < bytearray->capacity)
		bytearray->data[offset] = data;
}
void silly_bytearray_write(SillyByteArray *bytearray, size_t offset,
							void *data, size_t size)
{
	if (offset >= bytearray->capacity)
		return;
	if (offset + size >= bytearray->capacity)
		size = bytearray->capacity - offset;
	memcpy(bytearray->data + offset, data, size);
}
int silly_bytearray_append(SillyByteArray *bytearray, uint8_t data)
{
	int status;
	if (bytearray->length >= bytearray->capacity)
		if ((status = silly_bytearray_expand(bytearray, 1)) != SILLY_S_OK)
		{
			return status;
		}
	bytearray->data[bytearray->length++] = data;
	return SILLY_S_OK;
}
uint8_t silly_bytearray_read_byte(SillyByteArray *bytearray, size_t offset)
{
	if (offset >= bytearray->length)
		return 0;
	return bytearray->data[offset];
}
int silly_bytearray_read(SillyByteArray *bytearray, size_t offset, void *dest,
							size_t size)
{
	if (offset >= bytearray->length)
		return SILLY_S_MEMORY_ACCESS_OUT_OF_BOUNDS;
	if (size > bytearray->length - offset)
		size = bytearray->length - offset;
	memcpy(dest, bytearray->data + offset, size);
	return SILLY_S_OK;
}
int silly_bytearray_expand(SillyByteArray *bytearray, size_t size)
{
	bytearray->capacity = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
	if ((bytearray->data = (uint8_t *)realloc(bytearray->data,
												bytearray->capacity)) == NULL)
	{
		silly_bytearray_clean(bytearray);
		return SILLY_S_NO_MEMORY;
	}
	return SILLY_S_OK;
}
void silly_bytearray_clean(SillyByteArray *bytearray)
{
	if (bytearray != NULL)
	{
		if (bytearray->data != NULL)
			free(bytearray->data);
		bytearray->data = NULL;
		bytearray->length = bytearray->capacity = 0;
	}
}
