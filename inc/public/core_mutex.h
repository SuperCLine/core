/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_mutex.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-21      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_MUTEX_H_
#define _CORE_MUTEX_H_


__BEGIN_NAMESPACE


class CORE_API core_mutex
{
public:
	core_mutex(void);
	~core_mutex(void);

	void lock(void);
	void unlock(void);

private:
	struct mutex_impl* m_impl;
};

class CORE_API core_recursive_mutex
{
public:
	core_recursive_mutex(void);
	~core_recursive_mutex(void);

	void lock(void);
	void unlock(void);

private:
	struct mutex_recursive_impl* m_impl;
};

class CORE_API core_rdwt_mutex
{
public:
	core_rdwt_mutex(void);
	~core_rdwt_mutex(void);

	void lock_read(void);
	void unlock_read(void);
	void lock_write(void);
	void unlock_write(void);

private:
	struct mutex_rdwt_impl* m_impl;
};

class CORE_API core_rd_locker
{
public:
	explicit core_rd_locker(core_rdwt_mutex& mutex);
	~core_rd_locker(void);

private:
	core_rdwt_mutex& m_mutex;
};

class CORE_API core_wt_locker
{
public:
	explicit core_wt_locker(core_rdwt_mutex& mutex);
	~core_wt_locker(void);

private:
	core_rdwt_mutex& m_mutex;
};

template <class T>
class core_locker
{
public:
	explicit core_locker(T& mutex);
	~core_locker(void);

	void lock(void);
	void unlock(void);

private:
	T& m_mutex;
};

template <class T>
core_locker<T>::core_locker(T& mutex)
	: m_mutex(mutex)
{
	m_mutex.lock();
}

template <class T>
core_locker<T>::~core_locker(void)
{
	m_mutex.unlock();
}

template <class T>
void SuperCLine::core_locker<T>::lock(void)
{
	m_mutex.lock();
}

template <class T>
void SuperCLine::core_locker<T>::unlock(void)
{
	m_mutex.unlock();
}

struct TLockerTraits
{
	typedef core_mutex	mutex_type;
	typedef core_recursive_mutex mutex_recursive_type;
	typedef core_rdwt_mutex mutex_rdwt_type;

	typedef core_locker<core_mutex> locker_type;
	typedef core_locker<core_recursive_mutex> locker_recursive_type;
	
	typedef core_rd_locker locker_read_type;
	typedef core_wt_locker locker_write_type;
};

typedef TLockerTraits::mutex_type	umutex;
typedef TLockerTraits::locker_type	ulocker;

typedef TLockerTraits::mutex_recursive_type  umutex_recursive;
typedef TLockerTraits::locker_recursive_type ulocker_recursive;

typedef TLockerTraits::mutex_rdwt_type		umutex_rdwt;
typedef TLockerTraits::locker_read_type		ulocker_read;
typedef TLockerTraits::locker_write_type	ulocker_write;

__END_NAMESPACE


#endif //_CORE_MUTEX_H_