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
#include <cstring>
#include <cctype>
#include <cwchar>
#include <new>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <condition_variable>

#include <thread>


#define WIN32_LEAN_AND_MEAN
#define __DLL_EXPORT				__declspec(dllexport)
#define __DLL_IMPORT				__declspec(dllimport)
#define __LINK_LIB(import_lib)		comment(lib, import_lib)
#define __LINK_MSG(import_dll)		message("auto link " import_dll " ...")


__BEGIN_NAMESPACE

#define app_dir_separator '\\'
#define app_dir_separator_str "\\"
#define app_dir_dot '.'
#define app_dir_dot_str "."


#if defined(_UNICODE) || defined(UNICODE)
#define app_file_exist		_waccess
#else
#define app_file_exist		_access		// 0: exist 2:write-only 4:read-only 6:read and write
#endif

#define app_get_exedir ::GetModuleFileName
#define app_create_dir ::CreateDirectory

#define app_unreferenced(p) UNREFERENCED_PARAMETER(p)

__END_NAMESPACE


#endif //_CORE_PLATFORM_WINDOWS_H_