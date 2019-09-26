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
	CORE_API void init_core(interface_logmgr* log);
	CORE_API interface_logmgr* get_logger(void);
	CORE_API void core_log(ELogType type, const char* tag, const char* log);
	
};

template<typename... Args>
void core_logf(ELogType type, const char* tag, const char* format, Args... args)
{
	if (get_logger())
	{
		get_logger()->logf(type, tag, format, args...);
	}
}

__END_NAMESPACE


#endif //_CORE_ENGINE_H_