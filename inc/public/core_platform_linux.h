/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_platform_linux.h
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


#ifndef _CORE_PLATFORM_LINUX_H_
#define _CORE_PLATFORM_LINUX_H_


#include <cstdio>
#include <climits>
#include <exception>
#include <ctime>
#include <chrono>
#include <string>
#include <iostream>
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

#define __DLL_EXPORT 
#define __DLL_IMPORT 
#define __LINK_LIB(import_lib)		comment(lib, import_lib)
#define __LINK_MSG(import_dll)		message("auto link " import_dll " ...")


__BEGIN_NAMESPACE


#define app_dir_separator '/'
#define app_dir_separator_str "/"
#define app_dir_dot '.'
#define app_dir_dot_str "."


__END_NAMESPACE


#endif //_CORE_PLATFORM_LINUX_H_