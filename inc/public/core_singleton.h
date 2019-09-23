/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_singleton.h
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


#ifndef _CORE_SINGLETON_H_
#define _CORE_SINGLETON_H_


__BEGIN_NAMESPACE


template <typename T>
class core_singleton : public nocopyable
{
public:
	core_singleton(void)
	{
		app_assert(!m_singleton);
		m_singleton = app_cast_static(T*, this);
	}

	~core_singleton(void)
	{
		app_assert(m_singleton);
		m_singleton = 0;
	}

	static T& singleton(void)
	{
		app_assert(m_singleton);
		return (*m_singleton);
	}

	static T* singletonptr(void)
	{
		return m_singleton;
	}

protected:
	static T* m_singleton;
};

#define SINGLETON_DEFINE(T) template<> T* core_singleton<T>::m_singleton = 0;


__END_NAMESPACE


#endif //_CORE_SINGLETON_H_