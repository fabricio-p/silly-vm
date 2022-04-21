#ifndef PARSER_H
#define PARSER_H
#include "silly.h"
#include "macros.h"

/*
 * # SillyVM bytecode file format
 * *TODO* Put import section and maybe a constant pool
 *
 * U8 const signature[8]      "\0SillyVM"
 * U32 LE type_section_size
 * U32 LE type_count
 * U32 LE func_section_size
 * U32 LE func_count
 * U32 LE data_section_size
 * U32 LE data_count
 * U32 LE code_section_size
 * U32 LE __padding
 * U8 {section:     type}[type_section_size]
 * U8 {section: function}[func_section_size]
 * U8 {section:     data}[data_section_size]
 * U8 {section:     code}[code_section_size]
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
 * U16 LE local_count
 * U8     locals[local_count]
 * U8     name_length
 * U8     name[name_length]
 *
 * ## {section: code} format
 * U32 LE size
 * U8     bytecode[size]
 */

/* typedef struct SSegmentInfo {
  U32  size;
  U32  offset;
  Bool passive;
  U8   data[1];
} SInfoSegment; */

/* #define get_type_section(info)                              \
  (((SModule *)info)->raw.type_section.size != 0 ?          \
   &(((SModule *)info)->raw.data[                           \
     ((SModule *)info)->raw.type_section.offset]) : NULL)

#define get_function_section(info)                          \
  (((SModule *)info)->raw.function_section.size != 0 ?      \
   &(((SModule *)info)->raw.data[                           \
     ((SModule *)info)->raw.function_section.offset]) :     \
   NULL)

#define get_code_section(info)                              \
  (((SModule *)info)->raw.code_section.size != 0 ?          \
   &(((SModule *)info)->raw.data[                           \
     ((SModule *)info)->raw.code_section.offset]) : NULL)

#define get_data_section(info)                              \
  (((SModule *)info)->raw.code_section.size != 0 ?          \
   &(((SModule *)info)->raw.data[                           \
     ((SModule *)info)->raw.data_section.offset]) : NULL)
*/
SStatus parse_module(SEnv *, SModule *);
#define SECTION_PARSER(n)                                         \
  SStatus parse_##n##_section(SEnv *, SModule *, U8 const *const, \
                              SSecInfo const)
SECTION_PARSER(type);
SECTION_PARSER(data);
SECTION_PARSER(function);

#undef SECTION_PARSER

#endif /* PARSER_H */
