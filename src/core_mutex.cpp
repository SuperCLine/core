#include "stdafx.h"
#include "core_mutex.h"

#include <mutex>
#include <shared_mutex>

__BEGIN_NAMESPACE

struct mutex_impl
{
	mutable std::mutex mutex_;
};

struct mutex_recursive_impl
{
	mutable std::recursive_mutex mutex_;
};

struct mutex_rdwt_impl
{
	mutable std::shared_mutex mutex_;
};

//////////////////////////////////////////////////////////////////////////
//core_mutex
core_mutex::core_mutex(void)
	: m_impl(new mutex_impl())
{

}

core_mutex::~core_mutex(void)
{
	app_safe_delete(m_impl);
}

void core_mutex::lock(void)
{
	m_impl->mutex_.lock();
}

void core_mutex::unlock(void)
{
	m_impl->mutex_.unlock();
}

//////////////////////////////////////////////////////////////////////////
//core_recursive_mutex
core_recursive_mutex::core_recursive_mutex(void)
	: m_impl(new mutex_recursive_impl())
{

}

core_recursive_mutex::~core_recursive_mutex(void)
{
	app_safe_delete(m_impl);
}

void core_recursive_mutex::lock(void)
{
	m_impl->mutex_.lock();
}

void core_recursive_mutex::unlock(void)
{
	m_impl->mutex_.unlock();
}

//////////////////////////////////////////////////////////////////////////
//core_rdwt_mutex
core_rdwt_mutex::core_rdwt_mutex(void)
	: m_impl(new mutex_rdwt_impl())
{

}

core_rdwt_mutex::~core_rdwt_mutex(void)
{
	app_safe_delete(m_impl);
}

void core_rdwt_mutex::lock_read(void)
{
	m_impl->mutex_.lock_shared();
}

void core_rdwt_mutex::unlock_read(void)
{
	m_impl->mutex_.unlock_shared();
}

void core_rdwt_mutex::lock_write(void)
{
	m_impl->mutex_.lock();
}

void core_rdwt_mutex::unlock_write(void)
{
	m_impl->mutex_.unlock();
}

core_rd_locker::core_rd_locker(core_rdwt_mutex& mutex)
	: m_mutex(mutex)
{
	m_mutex.lock_read();
}

core_rd_locker::~core_rd_locker(void)
{
	m_mutex.unlock_read();
}

core_wt_locker::core_wt_locker(core_rdwt_mutex& mutex)
	: m_mutex(mutex)
{
	m_mutex.lock_write();
}

core_wt_locker::~core_wt_locker(void)
{
	m_mutex.unlock_write();
}

__END_NAMESPACE




