#include "silly_env.h"

int silly_env_init(SillyEnv *env, uint8_t page_count, size_t stack_size,
					size_t register_count, SillyStackSlot *registers)
{
	int status;
	if ((status = silly_memory_init(&env->linear_memory,
					page_count)) != SILLY_S_OK)
		return status;
	if ((status = silly_stack_init(&env->stack, stack_size, register_count,
														NULL)) != SILLY_S_OK)
		return status;
	memcpy(env->stack.data, registers,
			register_count * sizeof(SillyStackSlot));
	return SILLY_S_OK;
}
void silly_env_clean(SillyEnv *env)
{
	if (env != NULL)
	{
		silly_memory_clean(&env->linear_memory);
		silly_stack_clean(&env->stack);
	}
}
