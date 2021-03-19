/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\interface_log.h
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


#ifndef _INTERFACE_LOG_H_
#define _INTERFACE_LOG_H_

enum class ELogType :unsigned char
{
	Debug = 1,
	Info = 2,
	Error = 3,
	Perf = 4,
};

enum class ELoggerType :unsigned char
{
	Console = 0,
	FileDebug = 1,
	FileInfo = 2,
	FileError = 3,
	FilePerf = 4,
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