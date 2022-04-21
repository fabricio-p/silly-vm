#include "parser.h"
#include "internal.h"
#include "code.h"
#include "macros.h"
#include "code.h"
#define throw(err) { status = SILLY_E_##err; goto catch; }
#define throw_if(cond, err) if (cond) { throw(err); }
#define try(s) if ((status = (s)) != SILLY_E_OK) { goto catch; }

#define SECTION_PARSER(nm)                                            \
  SStatus parse_##nm##_section(SEnv *env, SModule *mod,               \
                               U8 const *const ptr, SSecInfo const info)

SECTION_PARSER(type)
{
  SStatus status = SILLY_E_OK;
  U8 const *const end = ptr + info.size;
  U8 const *type_ptr = ptr;
  Uint idx = 0;
  for (; idx < info.item_count && type_ptr < end; ++idx)
  {
    SType *type = STypeVec_push_empty(&(mod->types));
    Uint param_count = LOAD_U16(type_ptr);
    type_ptr += sizeof(U16);
    Uint result_count = LOAD_U16(type_ptr);
    type_ptr += sizeof(U16);
    Uint types_count = param_count + result_count;

    // TODO: Put some size checks so we don't read memory outside buffer
    //     : TODO: make it optional for custom builds for max startup
    //             speed and shit like that
    type->param_count  = param_count;
    type->result_count = result_count;
    type->types = type_ptr;
    
    type_ptr += types_count;
    // TODO: Create some more detailed error reporting system
    throw_if(type_ptr > end, INVALID_TYPE_SIZE);
  }
  throw_if(type_ptr != end, INCORRECT_SECTION_SIZE);
  throw_if(idx != info.item_count, INCORRECT_TYPE_COUNT);
catch:
  if (status != SILLY_E_OK)
  {
  }
  return status;
}

// TODO: Make functions to parse the other sections
//       TODO: Figure out how data section should be used and loaded

SECTION_PARSER(data)
{
  SStatus status = SILLY_E_OK;

  LOG_ERROR("Parser of data section is not implemented\n");
  exit(1);

// catch:
  return status;
}

SECTION_PARSER(function)
{
  SStatus status = SILLY_E_OK;
  void const *const end = ptr + info.size;
  void const *funcd_ptr = ptr;
  Uint idx = 0;
  for (; idx < info.item_count && funcd_ptr < end; ++idx)
  {
    SFunc *func = SFuncVec_push_empty(&(mod->functions));
    func->raw.offset = funcd_ptr - (void const *)ptr; // buffer;
    func->raw.type_idx = LOAD_U32(funcd_ptr);
    throw_if((Int)func->raw.type_idx >= STypeVec_len(mod->types),
             INVALID_TYPE_INDEX);
    func->type = &(mod->types[func->raw.type_idx]);
    funcd_ptr += sizeof(U32);

    func->raw.code_offset = LOAD_U32(funcd_ptr);
    funcd_ptr += sizeof(U32);

    // TODO: Check for overflows here
    func->locals.count = LOAD_U16(funcd_ptr);
    funcd_ptr += sizeof(U16);
    func->locals.types = func->locals.count == 0 ? NULL : funcd_ptr;
    funcd_ptr += func->locals.count;

    func->name.len = LOAD_U8(funcd_ptr++);
    func->name.str = funcd_ptr;
    funcd_ptr += func->name.len;
  }
  throw_if(funcd_ptr != end,       INCORRECT_SECTION_SIZE);
  throw_if(idx != info.item_count, INCORRECT_FUNCTION_COUNT);
catch:
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
