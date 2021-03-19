/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_platform_common.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-20      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_PLATFORM_COMMON_H_
#define _CORE_PLATFORM_COMMON_H_


__BEGIN_NAMESPACE

#undef TRUE
#undef FALSE
#undef NULL
#define TRUE 1
#define FALSE 0
#define NULL 0

using int8		= std::int8_t;
using int16		= std::int16_t;
using int32		= std::int32_t;
using int64		= std::int64_t;

using uint8		= std::uint8_t;
using uint16	= std::uint16_t;
using uint32	= std::uint32_t;
using uint64	= std::uint64_t;

using float32	= float;
using float64	= double;

using long32	= signed int;
using ulong32	= unsigned int;

using anichar	= char;
using unichar	= wchar_t;

using ustringc = std::string;
using ustringw = std::wstring;
using ustringstreamc = std::stringstream;
using ustringstreamw = std::wstringstream;

#if defined(UNICODE) || defined(_UNICODE)
using ustring = ustringw;
using ustringstream = ustringstreamw;
#else
using ustring = ustringc;
using ustringstream = ustringstreamc;
#endif

template <typename T>
using ulist = std::list<T>;
template <typename T>
using uvector = std::vector<T>;
template <typename KEY, typename VAL>
using umap = std::map<KEY, VAL>;
template <typename KEY, typename VAL>
using uset = std::set<KEY, VAL>;
template <typename T>
using udeque = std::deque<T>;
template <typename T>
using ushared_ptr = std::shared_ptr<T>;
template <typename KEY, typename VAL>
using umap_unordered = std::unordered_map<KEY, VAL>;

#define app_sscanf		std::sscanf		//string to any
#define app_vsscanf		std::vsscanf	//valist string to any
#define app_snprintf	std::snprintf	//any to string
#define app_vsnprintf	std::vsnprintf	//valist any to string

#define app_fopen		std::fopen
#define app_fread		std::fread
#define app_fseek       std::fseek
#define app_ftell       std::ftell
#define app_fclose		std::fclose

#define app_assert		assert
#define app_memset		std::memset
#define app_memcpy		std::memcpy
#define app_memcmp      std::memcmp
#define app_ceil		std::ceil
#define app_ceilf		std::ceilf
#define app_floor		std::floor
#define app_abs			std::abs
#define app_fabs		std::fabs


#if defined(UNICODE) || defined(_UNICODE)
#define app_strlen		std::wcslen
#define app_strcat		std::wcscat
#define app_strcpy		std::wcscpy
#define app_strrchr		std::wcsrchr

#define app_tolower		std::towlower
#define app_toupper		std::towupper

#define app_makestr(str)	L#str
#else
#define app_strlen		std::strlen
#define app_strcat		std::strcat
#define app_strcpy		std::strcpy
#define app_strrchr		std::strrchr

#define app_tolower		std::tolower
#define app_toupper		std::toupper

#define app_makestr(str)	#str
#endif

#define app_safe_delete(p)             { if (p) {delete (p);  (p) = NULL;} }
#define app_safe_delete_array(p)       { if (p) {delete [](p); (p) = NULL;} }
#define app_array_len(a)             ( sizeof(a) / sizeof(a[0]) )

#define app_cast_static(TYPE, EXPR) static_cast<TYPE>(EXPR)
#define app_cast_const(TYPE, EXPR) const_cast<TYPE>(EXPR)
#define app_cast_reinterpret(TYPE, EXPR) reinterpret_cast<TYPE>(EXPR)
#define app_cast_dynamic(TYPE, EXPR) dynamic_cast<TYPE>(EXPR)

#define app_make_pointer(val) app_cast_reinterpret(void*, app_cast_reinterpret(char*, 0) + (val))
#define app_make_value(pointer) app_cast_static(uint32, app_cast_reinterpret(char*, (pointer)) - app_cast_reinterpret(char*, 0))

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

__END_NAMESPACE


#endif //_CORE_PLATFORM_COMMON_H_