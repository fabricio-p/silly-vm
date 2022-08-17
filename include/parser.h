#ifndef PARSER_H
#define PARSER_H
#include "silly.h"
#include "macros.h"

/*
 * # SillyVM bytecode file format
 * *TODO* Put import section
 *
 * U8 const signature[8]      "\0SillyVM"
 * U32 LE type_section_size
 * U32 LE type_count
 * U32 LE func_section_size
 * U32 LE func_count
 * U32 LE data_section_size
 * U32 LE data_count
 * U32 LE code_section_size
 * U32 LE cpool_size
 * U8 {section:    cpool}[cpool_size * sizeof(U32)]
 * U8 {section:     type}[type_section_size]
 * U8 {section: function}[func_section_size]
 * U8 {section:     data}[data_section_size]
 * U8 {section:     code}[code_section_size]
 *
 * ## {section: cpool} format
 * U32 LE i64_count
 * U32 LE f64_count
 * U32 LE i32_count
 * U32 LE f32_count
 * U64 LE i64[i64_count]
 * F64 LE f64[f64_count]
 * U32 LE i32[i32_count]
 * F32 LE f32[f32_count]
 *
 * ## {section: type} format
 * <type>[]
 * ### <type> format
 * U16 LE param_count
 * U16 LE result_count
 * U8     types[param_count + result_count]
 *
 * ## {section: function} format
 * <function>[]
 * ### <function> format
 * U32 LE type_index
 * U32 LE code_offset
 * U32 LE code_size
 * U16 LE local_count
 * U8     locals[local_count]
 * U8     name_length
 * U8     name[name_length]
 *
 * ## {section: code} format
 * U32 LE size
 * U8     bytecode[size]
 */


SStatus parse_module(SEnv *, SModule *);
#define SECTION_PARSER(n)                                         \
  SStatus parse_##n##_section(SEnv *, SModule *, U8 const *const, \
                              SSecInfo const)
SECTION_PARSER(cpool);
SECTION_PARSER(type);
SECTION_PARSER(data);
SECTION_PARSER(function);
SECTION_PARSER(code);

#undef SECTION_PARSER

#endif /* PARSER_H */
