#include "parser.h"
#include "env.h"
#include "internal.h"
#include "code.h"
#include "macros.h"
#include "code.h"
#define throw(err) { status = SILLY_E_##err; goto catch; }
#define throw_if(cond, err) if (cond) { throw(err); }
#define try(s) if ((status = (s)) != SILLY_E_OK) { goto catch; }

#define SECTION_PARSER(nm)                                            \
  SStatus parse_##nm##_section(SEnv *env, SModule *mod,               \
                               U8 const *const ptr, U32 const size)

SECTION_PARSER(type)
{
  SStatus status = SILLY_E_OK;
  U8 const *const end = ptr + size;
  U32 count = 0;
  voidptr buffer = NULL;
  SType **types = mod->types;
  LOG_DEBUG("foo\n");
  if (types == NULL)
  {
    LOG_DEBUG("types is NULL\n");
    types = STypeVec_with_length(count);
  }
  LOG_DEBUG("\n");
  throw_if(types == NULL, OOM);
  LOG_DEBUG("\n");
  try(AllocChain_alloc(&(env->type_pool_chain), size, &buffer));
  LOG_DEBUG("\n");
  voidptr type_ptr = buffer;
  // U8 *type_ptr = buffer;
  U8 const *raw_type_ptr = ptr;
  LOG_EVAL(ptr, "p");
  LOG_EVAL(raw_type_ptr, "p");
  for (; raw_type_ptr < end; ++count)
  {
    U16 param_count  = LOAD_U16(raw_type_ptr);
    raw_type_ptr += sizeof(U16);
    U16 result_count = LOAD_U16(raw_type_ptr);
    raw_type_ptr += sizeof(U16);
    U32 types_count  = param_count + result_count;

    // TODO: Put some size checks so we don't read memory outside buffer
    SType *type = type_ptr;
    type->param_count  = param_count;
    type->result_count = result_count;
    memcpy(type->types, raw_type_ptr, types_count);
    
    raw_type_ptr += types_count;
    // TODO: Do some more detailed error reporting system
    throw_if(raw_type_ptr > end, PARSE_INVALID_TYPE_SIZE);

    type_ptr += offsetof(SType, types) + types_count;
  }
  throw_if(raw_type_ptr != end, PARSE_INVALID_TYPE_SIZE);
  LOG_EVAL(count, "lu");
  type_ptr = buffer;
  // imo better than indexing
  for (SType **current = types, **last = &(types[count]);
       current < last; ++current)
  {
    LOG_EVAL(type_ptr, "p");
    LOG_DEBUG("i = %lu\n", count - (last - current));
    *current = type_ptr;
    type_ptr += offsetof(SType, types) +
                ((SType *)type_ptr)->param_count +
                ((SType *)type_ptr)->result_count;
  }

catch:
  if (status != SILLY_E_OK)
  {
    if (buffer != NULL)
    {
      AllocChain_free(&(env->type_pool_chain));
    }
  }
  mod->loaded.types_buffer = buffer;
  mod->types = types;
  LOG_EVAL(status, "d");
  return status;
}

#if 0
SStatus parse_module(SEnv *env, SModule *mod)
{
  SStatus status = SILLY_S_OK;
  U8 *ptr = mod->raw.buffer;
  U32 len = Vector_U8_len(ptr);
  U8 *buff_end = ptr + len;
  try_eq(len, sizeof(mod->raw) + 8, MODULE_BELOW_MIN_SIZE);
  try_eq(memcmp(ptr, "\0SillyVM", 8), 0, MODULE_INCORRECT_SIGNATURE);
  mod->raw.size = len;
  ptr += 8;
  U32 type_sec_size = READ_U32(((U32 *)ptr)++);
  U32 func_sec_size = READ_U32(((U32 *)ptr)++);
  U32 code_sec_size = READ_U32(((U32 *)ptr)++);
  U32 data_sec_size = READ_U32(((U32 *)ptr)++);
  U8 const *calculated_end = ptr + type_sec_size;
  try_le(calculated_end, buff_end, OOB_SECTION_SIZE);
  try_le(calculated_end += func_sec_size, buff_end, OOB_SECTION_SIZE);
  try_le(calculated_end += code_sec_size, buff_end, OOB_SECTION_SIZE);
  try_le(calculated_end += data_sec_size, buff_end, OOB_SECTION_SIZE);

  _(parse_type_section(env, mod, &ptr, type_sec_size));
  _(parse_func_section(env, mod, &ptr, func_sec_size));
  _(parse_data_section(env, mod, &ptr, data_sec_size));
  _(parse_code_section(env, mod, &ptr, code_sec_size));

catch:
  return status;
}
#endif
