/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\interface_log.h
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


#ifndef _INTERFACE_LOG_H_
#define _INTERFACE_LOG_H_


enum ELogType
{
	ELT_NONE = 0,
	ELT_DEBUG,
	ELT_WARNING,
	ELT_ERROR,
};

class interface_log
{
public:
	virtual void log(ELogType type, const char* tag, const char* log) = 0;
	virtual void logf(ELogType type, const char* tag, const char* format, ...) = 0;
	virtual bool check(ELogType type, const char* tag) = 0;
};

class interface_logmgr
{
public:
	virtual bool init(const char* config) = 0;
	virtual void log(ELogType type, const char* tag, const char* log) = 0;
	virtual void logf(ELogType type, const char* tag, const char* format, ...) = 0;
};


#endif //_INTERFACE_LOG_H_