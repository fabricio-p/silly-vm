#ifndef SILLY_TABLES_H
#define SILLY_TABLES_H
#include "silly_common.h"
#include "leb128.h"
#include "hashing.h"

typedef struct _SillySymbol {
	char	  *name;
	size_t	  name_length;
	size_t	  hash;
	SillyFunc func_data;
} SillySymbol;

__inline void silly_symbol_init (SillySymbol *symbol, char *name,
									SillyFunc func_data)
{
	symbol->name = strdup(name);
	symbol->name_length = strlen(name);
	symbol->hash = silly_murmurHash2((uint8_t *)name, symbol->name_length, 0);
	symbol->func_data = func_data;
}
__inline void silly_symbol_clean(SillySymbol *symbol)
{
	if (symbol != NULL)
	{
		if (symbol->name != NULL)
			free(symbol->name);
		symbol->name = NULL;
		symbol->name_length = 0;
		symbol->hash = 0;
		memset(&symbol->func_data, 0, sizeof(SillyFunc));
	}
}

typedef struct _SillySymtable {
	SillySymbol *symbols;
	size_t		size;
	size_t		length;
	size_t		capacity;
} SillySymtable;

void		silly_symtable_init	   (SillySymtable *);
void		silly_symtable_register(SillySymtable *, char *, SillyFunc);
SillySymbol *silly_symtable_get	   (SillySymtable *, char *);
void		silly_symtable_clean   (SillySymtable *);

typedef SillySymtable SillyFunctable;

__inline SillyFunc *silly_functable_get_main(SillyFunctable *functable)
{
	return &silly_symtable_get((SillySymtable *)functable, "main")->func_data;
}

#endif /* SILLY_TABLES_H */
