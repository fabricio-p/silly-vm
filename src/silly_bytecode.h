#ifndef SILLY_BYTECODE_H
#define SILLY_BYTECODE_H
#include "silly_common.h"
#include "silly_reader.h"

enum silly_opcode {
	SILLY_OP_NOP = 0,
	SILLY_OP_CONST_I32_U,
	SILLY_OP_CONST_I32_S,
	SILLY_OP_CONST_I64_U,
	SILLY_OP_CONST_I64_S,
	SILLY_OP_CONST_F32,
	SILLY_OP_CONST_F64,
	SILLY_OP_RETURN
};

enum silly_section {
	SILLY_SEC_TEXT,
	SILLY_SEC_SYMBOLS,
	SILLY_SEC_TYPES,
	SILLY_SEC_FUNCTION,
	SILLY_SEC_MEMORY,
	SILLY_SEC_CODE
};

int silly_op_nop		(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_i32_u(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_i32_s(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_i64_u(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_i64_s(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_f32	(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_const_f64	(SillyStack *, SillyMemory *, SillyReader *);
int silly_op_return		(SillyStack *, SillyMemory *, SillyReader *);

#endif /* SILLY_BYTECODE_H */
