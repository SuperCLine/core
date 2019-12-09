/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_thread.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-21      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_THREAD_H_
#define _CORE_THREAD_H_

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#elif defined(__GNUC__)
#endif

__BEGIN_NAMESPACE


class CORE_API core_thread : private nocopyable
{
public:
	typedef std::function<void()> thread_func;

public:
	explicit core_thread(thread_func func);
	~core_thread(void);

	bool start(void);
	void stop(void);

	static void sleep(long32 ms);

private:
	static void process(core_thread* pthread);

private:
	thread_func m_thread_func;

	typedef ushared_ptr<std::thread> thread_ptr;
	thread_ptr m_thread_impl;
};


__END_NAMESPACE

#if defined(_MSC_VER)
#pragma warning(default : 4251)
#elif defined(__GNUC__)
#endif

#endif //_CORE_THREAD_H_