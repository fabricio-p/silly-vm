#include "silly_reader.h"
#include "silly_common.h"
#include <malloc.h>

void silly_reader_init(SillyReader *reader, const char *path, FILE *fd,
						int chunk_size)
{
	reader->path = strdup(path);
	reader->chunk_size = chunk_size;
	reader->dead = false;
	reader->fd = fd;
}
int silly_reader_open(SillyReader *reader, const char *path, int chunk_size)
{
	silly_reader_init(reader, path, fopen(path, "rb"),
			chunk_size == 0 ? 1024 << 3 : chunk_size);
	if (reader->fd == NULL)
	{
		silly_reader_close(reader);
		return SILLY_S_CANT_OPEN_FILE;
	}
	fstat(reader->fd->_file, &reader->stat);
	return SILLY_S_OK;
}
int silly_reader_read_to(SillyReader *reader, uint8_t *buffer, int size)
{
	if (size == -1)
		size = reader->chunk_size;
	if ((int)fread(buffer, 1, size, reader->fd) != size)
	{
		silly_reader_close(reader);
		return SILLY_S_CANT_WRITE;
	}
	return SILLY_S_OK;
}
uint8_t *silly_reader_read(SillyReader *reader, int size)
{
	uint8_t *buffer = (uint8_t *)malloc(size);
	if (buffer == NULL)
		return NULL;
	if ((int)fread(buffer, 1, size, reader->fd) != size)
	{
		free(buffer);
		return NULL;
	}
	return buffer;
}
void silly_reader_close(SillyReader *reader)
{
	if (reader != NULL) {
		if (reader->fd != NULL)
			fclose(reader->fd);
		if (reader->path != NULL)
			free(reader->path);
		reader->path = NULL;
		reader->chunk_size = 0;
		reader->dead = true;
		reader->fd = NULL;
		memset(&reader->stat, 0, sizeof(struct stat));
	}
}
