/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_dumper.h
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