#ifndef INTERNAL_H
#define INTERNAL_H
#define CVECTOR_POINTERMODE
#include <c-vector/lib.h> // CVECTOR_WITH_NAME, CVECTOR
#include "silly.h"
#include "macros.h"

CVECTOR_WITH_NAME(SType *, STypeVec);
CVECTOR_WITH_NAME(SValue,  SValueVec);
CVECTOR_WITH_NAME(SFunc *, SFuncVec);

#endif /* INTERNAL_H */
