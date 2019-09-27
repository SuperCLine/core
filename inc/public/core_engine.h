/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_engine.h
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


#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_


#if defined(_WIN32) || defined(WIN32)
	#include "core_platform_windows.h"
#else
	#include "core_platform_linux.h"
#endif
#include "core_platform_common.h"


__BEGIN_NAMESPACE


// TO CLine: add new and delete at later.
class CORE_API core
{

};

class CORE_API core_interface : public core
{
public:
	virtual ~core_interface(void) { }
};

class CORE_API core_property : core_interface
{
public:
	virtual ustring serialize(void) = 0;
	virtual bool deserialize(const ustring& name) = 0;
};

extern "C"
{
	CORE_API void load_module_core(interface_logmgr* log);
	CORE_API void unload_module_core(void);
};

__END_NAMESPACE


#endif //_CORE_ENGINE_H_