#ifndef SILLY_LOADER_H
#define SILLY_LOADER_H
#include "silly_tables.h"
#include "silly_env.h"
#include "silly_bytearray.h"

typedef struct _SillyLoader {
	SillyFunctable func_table;
	SillyType	   *types;
} SillyLoader;

typedef int (*SillyOpcodeExecutor)(SillyEnv *, SillyFunc *);

extern SillyOpcodeExecutor silly_opcode_map[];;

__silly_inline__
void silly_loader_init (SillyLoader *loader)
{
	silly_symtable_init(&loader->func_table);
}

int	 silly_loader_load_types	(SillyLoader *, SillyByteArray *);
int  silly_loader_load_symbols	(SillyLoader *, SillyByteArray *);
int  silly_loader_load_functions(SillyLoader *, SillyByteArray *);
int  silly_loader_load_memory	(SillyLoader *, SillyByteArray *);
int  silly_loader_load_code		(SillyLoader *, SillyByteArray *);

__silly_inline__
void silly_loader_clean(SillyLoader *loader)
{
	if (loader != NULL)
	{
		silly_symtable_clean(&loader->func_table);
	}
}

#endif /* SILLY_LOADER_H */
