#include "stdafx.h"
#include "core_waitgroup.h"

__BEGIN_NAMESPACE

struct waitgroup_impl
{
	std::mutex mutex_;
	std::atomic<int64> counter_;
	std::condition_variable cond_;
};

core_waitgroup::core_waitgroup(void)
	: m_impl(new waitgroup_impl())
{

}

core_waitgroup::~core_waitgroup(void)
{
	app_safe_delete(m_impl);
}

void core_waitgroup::add(int64 inc /*= 1*/)
{
	m_impl->counter_ += inc;
}

void core_waitgroup::done()
{
	if (--m_impl->counter_ <= 0)
	{
		m_impl->cond_.notify_all();
	}
}

void core_waitgroup::wait()
{
	std::unique_lock<std::mutex> locker(m_impl->mutex_);
	m_impl->cond_.wait(locker, [&] { return m_impl->counter_ <= 0; });
}

__END_NAMESPACE
