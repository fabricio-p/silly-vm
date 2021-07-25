#include "silly_bytecode.h"

int silly_op_nop(SillyEnv *env, SillyFunc *func)
{
	func->ip += sizeof(uintptr_t);
	return SILLY_S_OK;
}
int silly_op_const_i32_u(SillyEnv *env, SillyFunc *func)
{
	SillyStackSlot slot;
	slot.type = SILLY_T_I32_U;
	slot.value.i32_u = *(uint32_t *)(func->ip += sizeof(uintptr_t));
	func->ip += sizeof(uint32_t);
	return silly_stack_push(&env->stack, &slot) || SILLY_S_OK;	
}
int silly_op_const_i32_s(SillyEnv *env, SillyFunc *func)
{
	SillyStackSlot slot;
	slot.type = SILLY_T_I32_S;
	slot.value.i32_s = *(int32_t *)(func->ip += sizeof(uintptr_t));
	func->ip += sizeof(uintptr_t);
	return silly_stack_push(&env->stack, &slot) || SILLY_S_OK;
}
int silly_op_const_i64_u(SillyEnv *env, SillyFunc *func)
{
	SillyStackSlot slot;
	slot.type = SILLY_T_I64_U;
	slot.value.i64_u = *(uint64_t *)(func->ip += sizeof(uintptr_t));
	func->ip += 8;
	return silly_stack_push(&env->stack, &slot) || SILLY_S_OK;
}
int silly_op_const_i64_s(SillyEnv *env, SillyFunc *func)
{
	SillyStackSlot slot;
	slot.type = SILLY_T_I64_S;
	slot.value.i64_s = *(int64_t *)(func->ip += sizeof(uintptr_t));
	func->ip += 8;
	return silly_stack_push(&env->stack, &slot) || SILLY_S_OK;
}
int silly_op_return(SillyEnv *env, SillyFunc *func)
{
	SillyStackSlot slot;
	silly_stack_pop(&env->stack, SILLY_T_I32_U | SILLY_T_I32_S |
								 SILLY_T_I64_U | SILLY_T_I64_S, &slot);
	silly_stack_clear(&env->stack);
	silly_stack_push(&env->stack, &slot);
	return SILLY_S_OK;
}
