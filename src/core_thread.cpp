#include "stdafx.h"
#include "core_thread.h"

#include <thread>
#include <chrono>

__BEGIN_NAMESPACE

void core_thread::process(core_thread* pthread)
{
	if (pthread != NULL)
	{
		pthread->m_thread_func();
	}
}

core_thread::core_thread(thread_func func)
	: m_thread_func(std::move(func))
{

}

core_thread::~core_thread(void)
{

}

bool core_thread::start(void)
{
	if (m_thread_impl)
	{
		if (m_thread_impl->joinable())
		{
			core_log(ELT_ERROR, "core", "thread has not stopped yet.");
			return false;
		}
		m_thread_impl.reset();
	}

	m_thread_impl = std::make_shared<std::thread>(std::bind(process, this));

	return true;
}

void core_thread::stop(void)
{
	if (m_thread_impl)
	{
		m_thread_impl->join();
		m_thread_impl.reset();
	}
}

void core_thread::sleep(long32 ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

__END_NAMESPACE
