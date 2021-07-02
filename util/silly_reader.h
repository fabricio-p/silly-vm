#ifndef SILLY_READER_H
#define SILLY_READER_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "silly_common.h"

typedef struct _SillyReader {
	bool		dead;
	int			chunk_size;
	FILE		*fd;
	char		*path;
	struct stat stat;
} SillyReader;

void	silly_reader_init(SillyReader *, const char *, FILE *, int);
int		silly_reader_open(SillyReader *, const char *, int);
int		silly_reader_read_to(SillyReader *, uint8_t *, int);
uint8_t *silly_reader_read(SillyReader *, int);
void	silly_reader_close(SillyReader *);

__attribute__((always_inline)) static inline
uint8_t *silly_reader_read_all(const char *file)
{
	SillyReader reader;
	uint8_t *buffer;
	if (silly_reader_open(&reader, file, 0) != SILLY_S_OK)
		return NULL;
	buffer = (uint8_t *)malloc(reader.stat.st_size);
	if (buffer == NULL)
		return NULL;
	if (silly_reader_read_to(&reader, buffer,
				reader.stat.st_size) != SILLY_S_OK)
	{
		silly_reader_close(&reader);
		free(buffer);
		return NULL;
	}
	return buffer;
}

#endif /*SILLY_READER_H */
