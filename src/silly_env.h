#ifndef SILLY_ENV_H
#define SILLY_ENV_H
#include "silly_common.h"
#include "silly_tables.h"
#include <setjmp.h>

typedef struct _SillyEnv {
	SillyMemory    linear_memory;
	SillyStack	   stack;
	SillyFunctable functable;
	SillySymtable  symtable;
	jmp_buf		   jumper;
} SillyEnv;

int	 silly_env_init (SillyEnv *, uint8_t, size_t, size_t, SillyStackSlot *);
void silly_env_clean(SillyEnv *);

#endif /* SILLY_ENV_H */
