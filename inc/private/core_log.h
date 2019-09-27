/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\private\core_log.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-27      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_LOG_H_
#define _CORE_LOG_H_


void set_core_logger(interface_logmgr* log);
interface_logmgr* get_core_logger(void);

void core_log(ELogType type, const char* tag, const char* log);

template<typename... Args>
void core_logf(ELogType type, const char* tag, const char* format, Args... args)
{
	if (get_core_logger())
	{
		get_core_logger()->logf(type, tag, format, args...);
	}
}


#endif //_CORE_LOG_H_
