/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_platform_windows.h
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


#ifndef _CORE_PLATFORM_WINDOWS_H_
#define _CORE_PLATFORM_WINDOWS_H_

#include <SDKDDKVer.h>
#include <Windows.h>
#include <io.h>
#include <tchar.h>

#include <cstdio>
#include <climits>
#include <exception>
#include <ctime>
#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include <cmath>
#include <cassert>
#include <new>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <functional>
#include <condition_variable>


#define WIN32_LEAN_AND_MEAN
#define __DLL_EXPORT				__declspec(dllexport)
#define __DLL_IMPORT				__declspec(dllimport)
#define __LINK_LIB(import_lib)		comment(lib, import_lib)
#define __LINK_MSG(import_dll)		message("auto link " import_dll " ...")


__BEGIN_NAMESPACE


typedef signed char			int8;
typedef signed short		int16;
typedef signed int			int32;
typedef signed __int64		int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64    uint64;

typedef float				float32;
typedef double				float64;

typedef signed int			long32;
typedef unsigned int		ulong32;

typedef char				anichar;
typedef wchar_t				unichar;


#define app_strlen		_tcslen
#define app_strcat		_tcscat
#define app_strcpy		_tcscpy_s
#define app_strrchr		_tcsrchr

#define app_tolower		tolower
#define app_toupper		toupper
#define app_memset		memset
#define app_assert		assert
#define app_ceil		ceil
#define app_ceilf		ceilf
#define app_floor		floor
#define app_fabs		fabs


#define app_dir_separator '\\'
#define app_dir_separator_str "\\"
#define app_dir_dot '.'
#define app_dir_dot_str "."


#if defined(_UNICODE) || defined(UNICODE)
#define app_file_exist		_waccess
#define app_strmake(str)	L#str
#else
#define app_file_exist		_access		// 0: exist 2:write-only 4:read-only 6:read and write
#define app_strmake(str)	#str
#endif

#define app_get_exedir ::GetModuleFileName
#define app_create_dir ::CreateDirectory

__END_NAMESPACE


#endif //_CORE_PLATFORM_WINDOWS_H_