/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_nocopyable.h
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


#ifndef _CORE_NOCOPYABLE_H_
#define _CORE_NOCOPYABLE_H_


__BEGIN_NAMESPACE


class CORE_API nocopyable
{
public:
	nocopyable(void) {}

private:
	nocopyable(const nocopyable&);
	nocopyable& operator=(const nocopyable&);
};


__END_NAMESPACE


#endif //_CORE_NOCOPYABLE_H_