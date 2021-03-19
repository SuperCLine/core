/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_waitgroup.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2021-1-27      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_WAITGROUP_H_
#define _CORE_WAITGROUP_H_


__BEGIN_NAMESPACE


class CORE_API core_waitgroup : nocopyable
{
public:
	core_waitgroup(void);
	~core_waitgroup(void);

	void add(int64 inc = 1);
	void done();
	void wait();

private:
	struct waitgroup_impl* m_impl;
};


__END_NAMESPACE


#endif //_CORE_WAITGROUP_H_