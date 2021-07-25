#include "silly_loader.h"
#include "silly_bytecode.h"

SillyOpcodeExecutor silly_opcode_map[] = {
	&silly_op_nop,
	&silly_op_const_i32_u,
	&silly_op_const_i32_s,
	&silly_op_const_i64_u,
	&silly_op_const_i64_s,
	&silly_op_const_f32,
	&silly_op_const_f64,
	&silly_op_return
};
