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
	ELT_DEBUG = 1,
	ELT_INFO,
	ELT_ERROR,
	ELT_PERF,
};

enum ELoggerType
{
	ELT_COSOLE = 0,
	ELT_FILE_DEBUG,
	ELT_FILE_INFO,
	ELT_FILE_ERROR,
	ELT_FILE_PERF,
};

class interface_logger
{
public:
	virtual ~interface_logger(void) {}
	virtual ELoggerType log_type(void) = 0;
	virtual void log(ELogType type, const char* log) = 0;
};

class interface_logmgr
{
public:
	virtual ~interface_logmgr(void) {}
	virtual bool init(const char* config) = 0;
	virtual void log(ELogType type, const char* tag, const char* log) = 0;
	virtual void logf(ELogType type, const char* tag, const char* format, ...) = 0;
};


#endif //_INTERFACE_LOG_H_