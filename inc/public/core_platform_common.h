/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_platform_common.h
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


#ifndef _CORE_PLATFORM_COMMON_H_
#define _CORE_PLATFORM_COMMON_H_


__BEGIN_NAMESPACE


#undef TRUE
#undef FALSE
#undef NULL
#define TRUE 1
#define FALSE 0
#define NULL 0

typedef std::string			ustringc;
typedef std::wstring		ustringw;
typedef std::stringstream	ustringstreamc;
typedef std::wstringstream	ustringstreamw;

#if defined(UNICODE) || defined(_UNICODE)
typedef ustringw			ustring;
typedef ustringstreamw		utringstream;
#else
typedef ustringc			ustring;
typedef ustringstreamc		ustringstream;
#endif

#define ulist				std::list			
#define uvector				std::vector		
#define umap				std::map			
#define uset				std::set
#define udeque				std::deque
#define ushared_ptr			std::shared_ptr

#define app_sscanf			std::sscanf		//string to any
#define app_vsscanf			std::vsscanf	//valist string to any
#define app_snprintf		std::snprintf	//any to string
#define app_vsnprintf		std::vsnprintf	//valist any to string

#define app_safe_delete(p)		{ if (p) {delete (p);  (p) = NULL;} }
#define app_safe_delete_array(p)	{ if (p) {delete [](p); (p) = NULL;} }
#define app_array_len(a)             ( sizeof(a) / sizeof(a[0]) )

#define app_cast_static(TYPE, EXPR) static_cast<TYPE>(EXPR)
#define app_cast_const(TYPE, EXPR) const_cast<TYPE>(EXPR)
#define app_cast_reinterpret(TYPE, EXPR) reinterpret_cast<TYPE>(EXPR)
#define app_cast_dynamic(TYPE, EXPR) dynamic_cast<TYPE>(EXPR)

#define app_make_pointer(val) app_cast_reinterpret(void*, app_cast_reinterpret(char*, 0) + (val))
#define app_make_value(pointer) app_cast_static(uint32, app_cast_reinterpret(char*, (pointer)) - app_cast_reinterpret(char*, 0))


__END_NAMESPACE


#endif //_CORE_PLATFORM_COMMON_H_