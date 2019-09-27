/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_pool.h
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


#ifndef _CORE_POOL_H_
#define _CORE_POOL_H_


__BEGIN_NAMESPACE


template < class T >
class interface_pool_garbager
{
public:
	typedef typename ulist<T*>					pool_type;
	typedef typename ulist<T*>::iterator		iterator_type;
	typedef typename ulist<T*>::const_iterator	const_iterator_type;

public:
	virtual ~interface_pool_garbager(void) {}
	virtual void update(float32 tick, pool_type& pool) = 0;
};

// default implementation of pool garbage
template < typename T >
class core_pool_garbager : public interface_pool_garbager<T>
{
public:
	typedef typename interface_pool_garbager<T>::pool_type				pool_type;
	typedef typename interface_pool_garbager<T>::iterator_type			iterator_type;
	typedef typename interface_pool_garbager<T>::const_iterator_type	const_iterator_type;

public:
	// half an hour to garbage memory
	core_pool_garbager(size_t threshold = 0, float32 time = 1800.f)
		: m_threshold(threshold), m_total_time(time), m_cur_time(0.f)
	{}

	virtual void update(float32 tick, pool_type& pool)
	{
		m_cur_time += tick;

		if (m_cur_time > m_total_time)
		{
			m_cur_time = 0.f;

			// do garbage policy, here is garbage half of memory
			if (pool.size() > m_threshold)
			{
				size_t num = util_max<size_t>((size_t)((pool.size() - m_threshold) * 0.5), 1);

				iterator_type itr = pool.begin();
				for (size_t idx = 0; itr != pool.end() && idx < num; ++itr, ++idx)
				{
					app_safe_delete(*itr);
				}
				pool.erase(pool.begin(), itr);
			}
		}
	}

private:
	size_t	m_threshold;
	float32	m_total_time;
	float32	m_cur_time;
};

template <typename T, typename TMutexTraits = TLockerTraits>
class core_pool : private nocopyable
{
public:
	typedef typename T&  ref;
	typedef typename T*  pointer;

	typedef typename TMutexTraits::mutex_type	mutex_type;
	typedef typename TMutexTraits::locker_type	locker_type;

public:
	core_pool(void)
		: m_tag("Default"), m_ref(0)
	{

	}

	core_pool(const ustring& tag)
		: m_tag(tag), m_ref(0)
	{

	}

	~core_pool(void)
	{
		clear();
	}

	pointer get(void);
	void	cycle(pointer p);
	void	clear(void);
	void	set_tag(const ustring& tag);

	// pool memory garbage policy
	void	update(float32 tick, interface_pool_garbager<T>* garbager);

	// statistics for the size of memory used
	size_t  memory_size(void);

private:
	ulist<T*>		m_pool;

	mutex_type		m_mutex;

	// for memory leak assert
	ustring			m_tag;
	int32			m_ref;
};

template <typename T, typename TMutexTraits /*= TLockerTraits*/>
typename core_pool<T, TMutexTraits>::pointer core_pool<T, TMutexTraits>::get(void)
{
	locker_type locker(m_mutex);

	m_ref++;

	pointer p = 0;
	if (m_pool.empty())
	{
		p = new T();
	}
	else
	{
		p = m_pool.front();
		m_pool.pop_front();
	}

	return p;
}


template <typename T, typename TMutexTraits /*= TLockerTraits*/>
void core_pool<T, TMutexTraits>::cycle(pointer p)
{
	locker_type locker(m_mutex);

	m_ref--;

	m_pool.push_back(p);
}

template <typename T, typename TMutexTraits /*= TLockerTraits*/>
void core_pool<T, TMutexTraits>::clear(void)
{
	locker_type locker(m_mutex);

	app_assert(m_ref == 0);

	for (auto itr = m_pool.begin(); itr != m_pool.end(); ++itr)
	{
		app_safe_delete(*itr);
	}

	m_pool.clear();
}

template <typename T, typename TMutexTraits /*= TLockerTraits*/>
void core_pool<T, TMutexTraits>::set_tag(const ustring& tag)
{
	m_tag = tag;
}

template <typename T, typename TMutexTraits /*= TLockerTraits*/>
void core_pool<T, TMutexTraits>::update(float32 tick, interface_pool_garbager<T>* garbager)
{
	locker_type locker(m_mutex);

	if (garbager)
	{
		garbager->update(tick, m_pool);
	}
}


template <typename T, typename TMutexTraits /*= TLockerTraits*/>
size_t core_pool<T, TMutexTraits>::memory_size(void)
{
	locker_type locker(m_mutex);

	return m_pool.size() * sizeof(T);
}

__END_NAMESPACE


#endif //_CORE_POOL_H_