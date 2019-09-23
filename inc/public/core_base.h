/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_base.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-20      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_BASE_H_
#define _CORE_BASE_H_


#define __BEGIN_NAMESPACE namespace SuperCLine {
#define __END_NAMESPACE }
#define __USE_NAMESPACE using namespace SuperCLine;


#define app_max_path		(260)
#define app_index_null		(-1)

#define app_int8_min		(-127i8 - 1)
#define app_int8_max		(127i8)
#define app_uint8_max		0xffui8
#define app_int16_min		(-32767i16 - 1)
#define app_int16_max		(32767i16)
#define app_uint16_max		0xffffui16
#define app_int32_min		(-2147483647i32 - 1)
#define app_int32_max		(2147483647i32)
#define app_uint32_max		0xffffffffui32
#define app_int64_min		(-9223372036854775807i64 - 1)
#define app_int64_max		(9223372036854775807i64)
#define app_uint64_max		0xffffffffffffffffui64
#define app_long32_min		(-2147483647L - 1)
#define app_long32_max		(2147483647L)
#define app_ulong32_max		0xffffffffUL


#pragma 
#endif //_CORE_BASE_H_