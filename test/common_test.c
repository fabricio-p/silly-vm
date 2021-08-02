#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include "silly_common.h"
#include "testing.h"

static SillyStack stack;

int stack_suite_init(void)
{
	return (silly_stack_init(&stack, 8, 0, NULL) || CUE_SUCCESS) &&
		CUE_NOMEMORY;
}
int stack_suite_cleanup(void)
{
	silly_stack_clean(&stack);
	return CUE_SUCCESS;
}

void test_stack_push(void)
{
	silly_stack_dump(&stack);
	SillyStackSlot slot = {
		.type = SILLY_T_I32_U,
		.value = { .i32_u = 42 }
	};

	silly_stack_push(&stack, &slot);

	CU_ASSERT_EQUAL_FATAL		(stack.size, 8 * sizeof(SillyStackSlot));
	CU_ASSERT_EQUAL_FATAL		(stack.count, 1);
	CU_ASSERT_EQUAL_FATAL		(stack.slot_count, 8);
	CU_ASSERT_EQUAL_FATAL		(stack.register_count, 0);
	CU_ASSERT_PTR_NULL_FATAL	(stack.symbol);
	CU_ASSERT_PTR_NULL_FATAL	(stack.parent);
	CU_ASSERT_PTR_NULL_FATAL	(stack.next);
	CU_ASSERT_PTR_NOT_NULL_FATAL(stack.data);
	CU_ASSERT_EQUAL_FATAL		(stack.data_start, stack.data);
	CU_ASSERT_PTR_EQUAL_FATAL	(stack.stack_top, stack.data_start + 1);
}
void test_stack_pop(void)
{
	silly_stack_dump(&stack);
	SillyStackSlot _slot = {
		.type = SILLY_T_I64_S,
		.value = { .i64_s = 0x3acbefdeadbeef }
	};
	SillyStackSlot slot;
	
	silly_stack_push(&stack, &_slot);
	silly_stack_pop(&stack, SILLY_T_I64_S, &slot);

	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I64_S);
	CU_ASSERT_EQUAL_FATAL(slot.value.i64_s, _slot.value.i64_s);
	CU_ASSERT_EQUAL_FATAL(stack.count, 0);
}
void test_stack_clear(void)
{
	silly_stack_dump(&stack);

	SillyStackSlot slot;

	silly_stackslot_init(&slot, SILLY_T_F64, (SillyStackValue)45.24);
	silly_stack_push(&stack, &slot);

	silly_stackslot_init(&slot, SILLY_T_I32_S, (SillyStackValue)364);
	silly_stack_push(&stack, &slot);

	CU_ASSERT_EQUAL_FATAL(stack.count, 2);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.stack_top, stack.data_start + 2);
	
	silly_stack_clear(&stack);

	CU_ASSERT_EQUAL_FATAL	 (stack.count, 0);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.stack_top, stack.data_start);
}
void test_stack_drop(void)
{
	silly_stack_dump(&stack);

	SillyStackSlot slot;

	silly_stackslot_init(&slot, SILLY_T_I64_U, (SillyStackValue)35345);
	silly_stack_push(&stack, &slot);
	
	silly_stackslot_init(&slot, SILLY_T_F32, (SillyStackValue)34.5);
	silly_stack_push(&stack, &slot);

	CU_ASSERT_EQUAL_FATAL(stack.count, 2);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.stack_top, stack.data_start + 2);

	silly_stack_drop(&stack);

	CU_ASSERT_EQUAL_FATAL	 (stack.count, 1);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.stack_top, stack.data_start + 1);
}

void test_stack_split_n_link(void)
{
	silly_stack_dump(&stack);

	SillyStack new_stack;
	SillyStackSlot slot;
	uint8_t type_block[offsetof(SillyType, types) + 3];
	SillyType *type = (SillyType *)type_block;
	
	memset(type_block, 0, sizeof(type_block));
	type->param_count = 3;
	type->types[0]	  = SILLY_T_I32_U;
	type->types[1]	  = SILLY_T_I32_S;
	type->types[2]	  = SILLY_T_F64;
	
	silly_stackslot_init(&slot, SILLY_T_I32_U, (SillyStackValue)36355);
	silly_stack_push(&stack, &slot);
	
	silly_stackslot_init(&slot, SILLY_T_I32_S, (SillyStackValue)8557);
	silly_stack_push(&stack, &slot);
	
	silly_stackslot_init(&slot, SILLY_T_F64, (SillyStackValue)394.546);
	silly_stack_push(&stack, &slot);

	new_stack = silly_stack_split(&stack, type, NULL);

	CU_ASSERT_EQUAL_FATAL	 (new_stack.register_count, 3);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.size, 8 * sizeof(SillyStackSlot));
	CU_ASSERT_EQUAL_FATAL	 (new_stack.count, 0);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.slot_count, 8);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.register_count, 3);
	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.data_start, stack.stack_top);
	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.stack_top, stack.stack_top);

	silly_stack_link(&stack, &new_stack);

	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.parent, &stack);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.next, &new_stack);

	silly_stack_clean(&new_stack);
}

int main(int argc, char **argv)
{
	int status = 0;
	CU_initialize_registry();
	CU_TestInfo stack_tests[] = {
	{ "test_push",		   test_stack_push		   },
	{ "test_pop",		   test_stack_pop		   },
	{ "test_clear",		   test_stack_clear		   },
	{ "test_drop",		   test_stack_drop		   },
	{ "test_split_n_link", test_stack_split_n_link },
	CU_TEST_INFO_NULL
	};
	CU_SuiteInfo suites[] = {
		{ "SillyStack", stack_suite_init, stack_suite_cleanup, NULL, NULL,
			stack_tests },
		CU_SUITE_INFO_NULL
	};
	if ((status = CU_register_suites(suites)) != CUE_SUCCESS)
		goto cleanup;
	RUN_TESTS
cleanup:
	CU_cleanup_registry();
	return status;
}
