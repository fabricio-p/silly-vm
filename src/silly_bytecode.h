#ifndef SILLY_BYTECODE_H
#define SILLY_BYTECODE_H
#include "silly_common.h"
#include "silly_env.h"

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
	SILLY_SEC_TYPE,
	SILLY_SEC_SYMBOL,
	SILLY_SEC_FUNCTION,
	SILLY_SEC_MEMORY,
	SILLY_SEC_CODE
};

int silly_op_nop		(SillyEnv *, SillyFunc *);
int silly_op_const_i32_u(SillyEnv *, SillyFunc *);
int silly_op_const_i32_s(SillyEnv *, SillyFunc *);
int silly_op_const_i64_u(SillyEnv *, SillyFunc *);
int silly_op_const_i64_s(SillyEnv *, SillyFunc *);
int silly_op_const_f32	(SillyEnv *, SillyFunc *);
int silly_op_const_f64	(SillyEnv *, SillyFunc *);
int silly_op_return		(SillyEnv *, SillyFunc *);

#endif /* SILLY_BYTECODE_H */
