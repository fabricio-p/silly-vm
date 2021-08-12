#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include "silly_common.h"
#include "testing.h"

/* Stack test suite */
static SillyStack stack;

int stack_suite_init(void)
{
	return silly_stack_init(&stack, 8, 0, NULL) ? CUE_NOMEMORY : CUE_SUCCESS;
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

	new_stack = silly_stack_split(&stack, type, NULL, 0);

	CU_ASSERT_EQUAL_FATAL	 (new_stack.register_count, 3);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.size, 8 * sizeof(SillyStackSlot));
	CU_ASSERT_EQUAL_FATAL	 (new_stack.count, 0);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.slot_count, 8);
	CU_ASSERT_EQUAL_FATAL	 (new_stack.register_count, 3);
	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.data_start, stack.stack_top + 3);
	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.stack_top - 3, stack.stack_top);

	silly_stack_link(&stack, &new_stack);

	CU_ASSERT_PTR_EQUAL_FATAL(new_stack.parent, &stack);
	CU_ASSERT_PTR_EQUAL_FATAL(stack.next, &new_stack);

	silly_stack_clean(&new_stack);
}

/* Memory test suite */
static SillyMemory memory;
int memory_suite_init(void)
{
	return silly_memory_init(&memory, 4) ? CUE_NOMEMORY : CUE_SUCCESS;
}
int memory_suite_cleanup(void)
{
	silly_memory_clean(&memory);
	return CUE_SUCCESS;
}
void test_memory_init(void)
{
	CU_ASSERT_EQUAL_FATAL(memory.page_count, 4);
	CU_ASSERT_EQUAL_FATAL(memory.size, 0x10000 << 2);
	CU_ASSERT_PTR_NOT_NULL_FATAL(memory.data);
}
void test_memory_write(void)
{
	int hello_size = sizeof("Hello, world!");
	silly_memory_write(&memory, 0, "Hello, world!", hello_size);
	CU_ASSERT_STRING_EQUAL_FATAL(memory.data, "Hello, world!");
}
void test_memory_read(void)
{
	int hello_size = sizeof("Hello, world!");
	silly_memory_write(&memory, 0, "Hello, world!", hello_size);
	char copy_space[hello_size];
	silly_memory_read(&memory, 0, copy_space, hello_size);
	CU_ASSERT_STRING_EQUAL_FATAL(copy_space, "Hello, world!");
}
void test_memory_store(void)
{
	silly_memory_store(&memory, 0, SILLY_MT_I8_U, (SillyStackValue)8);
	CU_ASSERT_EQUAL_FATAL(*(uint8_t *)memory.data, 8);
	silly_memory_store(&memory, 0, SILLY_MT_I8_S, (SillyStackValue)-46);
	CU_ASSERT_EQUAL_FATAL(*(int8_t *)memory.data, -46);
	silly_memory_store(&memory, 0, SILLY_MT_I16_U, (SillyStackValue)366);
	CU_ASSERT_EQUAL_FATAL(*(uint16_t *)memory.data, 366);
	silly_memory_store(&memory, 0, SILLY_MT_I16_S, (SillyStackValue)-256);
	CU_ASSERT_EQUAL_FATAL(*(int16_t *)memory.data, -256);
	silly_memory_store(&memory, 0, SILLY_MT_I32_U, (SillyStackValue)5465435);
	CU_ASSERT_EQUAL_FATAL(*(uint32_t *)memory.data, 5465435);
	silly_memory_store(&memory, 0, SILLY_MT_I32_S, (SillyStackValue)-46535);
	CU_ASSERT_EQUAL_FATAL(*(int32_t *)memory.data, -46535);
	silly_memory_store(&memory, 0, SILLY_MT_I64_U,
						(SillyStackValue)4554564356765);
	CU_ASSERT_EQUAL_FATAL(*(uint64_t *)memory.data, 4554564356765);
	silly_memory_store(&memory, 0, SILLY_MT_I64_S,
						(SillyStackValue)(int64_t)-367393881);
	CU_ASSERT_EQUAL_FATAL(*(int64_t *)memory.data, -367393881);


	silly_memory_store(&memory, 0, SILLY_MT_F32,
						(SillyStackValue)(float)46.24);
	CU_ASSERT_EQUAL_FATAL(*(float *)memory.data, (float)46.24);
	silly_memory_store(&memory, 0, SILLY_MT_F64,
						(SillyStackValue)(double)3644.25356);
	CU_ASSERT_EQUAL_FATAL(*(double *)memory.data, 3644.25356);
}
void test_memory_load(void)
{
	memset(memory.data, 0, memory.size);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 0, SILLY_MT_I8_U, (SillyStackValue)215);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 1, SILLY_MT_I8_S, (SillyStackValue)-37);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 2, SILLY_MT_I16_U, (SillyStackValue)834);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 4, SILLY_MT_I16_S, (SillyStackValue)-305);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 6, SILLY_MT_I32_U, (SillyStackValue)288829);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 10, SILLY_MT_I32_S, (SillyStackValue)-35351);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 14, SILLY_MT_F32,
						(SillyStackValue)(float)35.12);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 18, SILLY_MT_I64_U, (SillyStackValue)9999999);
	printf("\n%d\n", __LINE__);
	silly_memory_store(&memory, 26, SILLY_MT_I64_S, (SillyStackValue)-353454);
	printf("\n%d\n", __LINE__);
	/* silly_memory_store(&memory, 34, SILLY_MT_F64,
						(SillyStackValue)(double)45.928487); */
	printf("\n%d\n", __LINE__);


	SillyStackSlot slot;
	printf("\n%d\n", __LINE__);
	silly_memory_load(&memory, 0, SILLY_MT_I8_U, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_U);
	CU_ASSERT_EQUAL_FATAL((uint8_t)slot.value.i32_u, 215);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 1, SILLY_MT_I8_S, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_S);
	CU_ASSERT_EQUAL_FATAL((int8_t)slot.value.i32_s, -37);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 2, SILLY_MT_I16_U, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_U);
	CU_ASSERT_EQUAL_FATAL((uint16_t)slot.value.i32_u, 834);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 4, SILLY_MT_I16_S, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_S);
	CU_ASSERT_EQUAL_FATAL((int16_t)slot.value.i32_s, -305);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 6, SILLY_MT_I32_U, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_U);
	CU_ASSERT_EQUAL_FATAL(slot.value.i32_u, 288829);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 10, SILLY_MT_I32_S, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I32_U);
	CU_ASSERT_EQUAL_FATAL(slot.value.i32_s, -35351);
	printf("\n%d\n", __LINE__);

	/* silly_memory_load(&memory, 14, SILLY_MT_F32, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_F32);
	CU_ASSERT_EQUAL_FATAL(slot.value.f32, (float)35.12);
	printf("\n%d\n", __LINE__);*/

	silly_memory_load(&memory, 18, SILLY_MT_I64_U, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I64_U);
	CU_ASSERT_EQUAL_FATAL(slot.value.i64_u, 9999999);
	printf("\n%d\n", __LINE__);

	silly_memory_load(&memory, 26, SILLY_MT_I64_S, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_I64_S);
	CU_ASSERT_EQUAL_FATAL(slot.value.i64_s, -353454);
	printf("\n%d\n", __LINE__);

	/* silly_memory_load(&memory, 34, SILLY_MT_F64, &slot);
	CU_ASSERT_EQUAL_FATAL(slot.type, SILLY_T_F64);
	CU_ASSERT_EQUAL_FATAL(slot.value.f64, 45.928487);
	printf("\n%d\n", __LINE__);*/
}

int main(int argc, char **argv)
{
	int status = 0;
	CU_initialize_registry();
	CU_TestInfo stack_tests[] = {
		{ "push",		   test_stack_push		   },
		{ "pop",		   test_stack_pop		   },
		{ "clear",		   test_stack_clear		   },
		{ "drop",		   test_stack_drop		   },
		{ "split n'link", test_stack_split_n_link },
		CU_TEST_INFO_NULL
	};
	CU_TestInfo memory_tests[] = {
		{ "init",  test_memory_init  },
		{ "write", test_memory_write },
		{ "read",  test_memory_read  },
		{ "store", test_memory_store },
		{ "load",  test_memory_load  },
		CU_TEST_INFO_NULL
	};
	CU_SuiteInfo suites[] = {
		{ "SillyStack", stack_suite_init, stack_suite_cleanup,
			NULL, NULL, stack_tests },
		{ "SillyMemory", memory_suite_init, memory_suite_cleanup,
			NULL, NULL, memory_tests },
		CU_SUITE_INFO_NULL
	};
	if ((status = CU_register_suites(suites)) != CUE_SUCCESS)
		goto cleanup;
	RUN_TESTS
cleanup:
	CU_cleanup_registry();
	return status;
}
