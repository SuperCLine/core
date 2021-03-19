/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_dumper.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       : vim /etc/profile
|				ulimit -S -c unlimited > /dev/null2>&1.ok
|				source /etc/profile
|				ulimit -c
|				echo "/dump/core-%e-%p-%t" > /proc/sys/kernel/core_pattern
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-20      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_DUMPER_H_
#define _CORE_DUMPER_H_


__BEGIN_NAMESPACE


class CORE_API core_dumper
{
public:
	core_dumper(const char* dump_path, const char* dump_file);
	
};


__END_NAMESPACE


#endif //_CORE_DUMPER_H_