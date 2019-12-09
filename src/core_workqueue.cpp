#include "stdafx.h"
#include "core_workqueue.h"


__BEGIN_NAMESPACE

uint16 core_workqueue::get_channel(const ustring& channel_name)
{
	ulocker_recursive scoped_lock(m_channel_map_mutex);

	channel_itr_type itr = m_channel_map.find(channel_name);
	if (itr == m_channel_map.end())
	{
		itr = m_channel_map.insert(channel_map_type::value_type(channel_name, m_next_channel++)).first;
	}

	return itr->second;
}

core_workqueue::request::request(uint16 channel, uint16 rtype, void* rdata, uint8 retry, request_id_type rid)
: m_channel(channel), m_type(rtype), m_data(rdata), m_retry_count(retry), m_id(rid), m_aborted(false)
{

}

core_workqueue::request::~request(void)
{

}


core_workqueue::response::response(const request* rq, bool success, void* data, bool owndata /*= true*/, const ustring& msg /*= ""*/)
: m_success(success), m_own_data(owndata), m_request(rq), m_data(data), m_messages(msg)
{

}

core_workqueue::response::~response(void)
{
	app_safe_delete(m_request);
}

void core_workqueue::response::abort_request(void)
{
	m_request->abort_request();
	if (m_own_data)
		app_safe_delete(m_data);
}

core_default_workqueue_base::core_default_workqueue_base(const ustring& name /*= ""*/)
: m_name(name)
, m_is_running(false)
, m_response_processing_timelimit(8)
, m_request_count(0)
, m_paused(false)
, m_accept_requests(true)
{

}

core_default_workqueue_base::~core_default_workqueue_base(void)
{
	for (request_queue_type::iterator itr = m_request_queue.begin(); itr != m_request_queue.end(); ++itr)
	{
		app_safe_delete(*itr);
	}
	m_request_queue.clear();

	for (response_queue_type::iterator itr = m_response_queue.begin(); itr != m_response_queue.end(); ++itr)
	{
		app_safe_delete(*itr);
	}
	m_response_queue.clear();
}

void core_default_workqueue_base::add_request_handler(uint16 channel, request_handler* rh)
{
	ulocker_write scoped_lock(m_request_handler_mutex);

	request_handler_list_bychannel_type::iterator itr = m_request_handlers.find(channel);
	if (itr == m_request_handlers.end())
		itr = m_request_handlers.insert(request_handler_list_bychannel_type::value_type(channel, request_handler_list_type())).first;

	request_handler_list_type& handlers = itr->second;
	bool duplicate = false;
	for (request_handler_list_type::iterator jtr = handlers.begin(); jtr != handlers.end(); ++jtr)
	{
		if ((*jtr)->get_handler() == rh)
		{
			duplicate = true;
			break;
		}
	}

	if (!duplicate)
	{
		handlers.push_back(request_handler_holder_ptr_type(new request_handler_holder(rh)));
	}
}

void core_default_workqueue_base::remove_request_handler(uint16 channel, request_handler* rh)
{
	ulocker_write scoped_lock(m_request_handler_mutex);

	request_handler_list_bychannel_type::iterator itr = m_request_handlers.find(channel);
	if (itr != m_request_handlers.end())
	{
		request_handler_list_type& handlers = itr->second;
		for (request_handler_list_type::iterator jtr = handlers.begin(); jtr != handlers.end(); ++jtr)
		{
			if ((*jtr)->get_handler() == rh)
			{
				(*jtr)->disconnect_handler();
				handlers.erase(jtr);
				break;
			}
		}
	}
}

void core_default_workqueue_base::add_response_handler(uint16 channel, response_handler* rh)
{
	response_handler_list_bychannel_type::iterator itr = m_response_handlers.find(channel);
	if (itr == m_response_handlers.end())
	{
		itr = m_response_handlers.insert(response_handler_list_bychannel_type::value_type(channel, response_handler_list_type())).first;
	}

	response_handler_list_type& handlers = itr->second;
	if (std::find(handlers.begin(), handlers.end(), rh) == handlers.end())
	{
		handlers.push_back(rh);
	}
}

void core_default_workqueue_base::remove_response_handler(uint16 channel, response_handler* rh)
{
	response_handler_list_bychannel_type::iterator itr = m_response_handlers.find(channel);
	if (itr != m_response_handlers.end())
	{
		response_handler_list_type& handlers = itr->second;
		response_handler_list_type::iterator jtr = std::find(handlers.begin(), handlers.end(), rh);
		if (jtr != handlers.end())
		{
			handlers.erase(jtr);
		}
	}
}

core_workqueue::request_id_type core_default_workqueue_base::add_request(uint16 channel, uint16 rtype, void* rdata, uint8 retry_count /*= 0*/, bool force_synchronous /*= false*/)
{
	request* req = 0;
	request_id_type rid = 0;

	{
		// lock to acquire rid and push request to the queue
		ulocker_recursive scoped_lock(m_request_mutex);

		if (!m_accept_requests || m_shuttingdown)
			return 0;

		rid = ++m_request_count;
		req = new request(channel, rtype, rdata, retry_count, rid);

		if (!force_synchronous)
		{
			m_request_queue.push_back(req);
			notify_workers();
			return rid;
		}
	}

	process_request_response(req, true);

	return rid;
}

void core_default_workqueue_base::add_request_with_id(request_id_type rid, uint16 channel, uint16 rtype, void* rdata, uint8 retry_count)
{
	ulocker_recursive scoped_lock(m_request_mutex);

	if (m_shuttingdown) 
		return;

	request* req = new request(channel, rtype, rdata, retry_count, rid);

#if 1 // thread mode
	m_request_queue.push_back(req);
	notify_workers();
#else
	process_request_response(req, true);
#endif
}

void SuperCLine::core_default_workqueue_base::abort_request(request_id_type id)
{
	ulocker_recursive scoped_lock(m_process_mutex);
	for (request_queue_type::iterator itr = m_process_queue.begin(); itr != m_process_queue.end(); ++itr)
	{
		if ((*itr)->get_id() == id)
		{
			(*itr)->abort_request();
			break;
		}
	}

	{
		ulocker_recursive scoped_lock_rq(m_request_mutex);
		for (request_queue_type::iterator itr = m_request_queue.begin(); itr != m_request_queue.end(); ++itr)
		{
			if ((*itr)->get_id() == id)
			{
				(*itr)->abort_request();
				break;
			}
		}
	}

	{
		ulocker_recursive scoped_lock_rp(m_response_mutex);
		for (response_queue_type::iterator itr = m_response_queue.begin(); itr != m_response_queue.end(); ++itr)
		{
			if ((*itr)->get_request()->get_id() == id)
			{
				(*itr)->abort_request();
				break;
			}
		}
	}

}

void SuperCLine::core_default_workqueue_base::abort_requests_by_channel(uint16 channel)
{
	ulocker_recursive scoped_lock(m_process_mutex);
	for (request_queue_type::iterator itr = m_process_queue.begin(); itr != m_process_queue.end(); ++itr)
	{
		if ((*itr)->get_channel() == channel)
		{
			(*itr)->abort_request();
		}
	}

	{
		ulocker_recursive scoped_lock_rq(m_request_mutex);
		for (request_queue_type::iterator itr = m_request_queue.begin(); itr != m_request_queue.end(); ++itr)
		{
			if ((*itr)->get_channel() == channel)
			{
				(*itr)->abort_request();
			}
		}
	}

	{
		ulocker_recursive scoped_lock_rp(m_response_mutex);
		for (response_queue_type::iterator itr = m_response_queue.begin(); itr != m_response_queue.end(); ++itr)
		{
			if ((*itr)->get_request()->get_channel() == channel)
			{
				(*itr)->abort_request();
			}
		}
	}
}

void SuperCLine::core_default_workqueue_base::abort_all_requests(void)
{
	ulocker_recursive scoped_lock(m_process_mutex);
	for (request_queue_type::iterator itr = m_process_queue.begin(); itr != m_process_queue.end(); ++itr)
	{
		(*itr)->abort_request();
	}

	{
		ulocker_recursive scoped_lock_rq(m_request_mutex);
		for (request_queue_type::iterator itr = m_request_queue.begin(); itr != m_request_queue.end(); ++itr)
		{
			(*itr)->abort_request();
		}
	}

	{
		ulocker_recursive scoped_lock_rp(m_response_mutex);
		for (response_queue_type::iterator itr = m_response_queue.begin(); itr != m_response_queue.end(); ++itr)
		{
			(*itr)->abort_request();
		}
	}
}

void core_default_workqueue_base::set_paused(bool pause)
{
	ulocker_recursive scoped_lock(m_request_mutex);
	m_paused = pause;
}

void core_default_workqueue_base::set_requests_accepted(bool accept)
{
	ulocker_recursive scoped_lock(m_request_mutex);
	m_accept_requests = accept;
}

void core_default_workqueue_base::_process_next_request()
{
	request* rq = 0;

	{
		ulocker_recursive scoped_lock(m_process_mutex);
		{
			ulocker_recursive scoped_lock_rq(m_request_mutex);
			if (!m_request_queue.empty())
			{
				rq = m_request_queue.front();
				m_request_queue.pop_front();
				m_process_queue.push_back(rq);
			}
		}
	}

	if (rq)
	{
		process_request_response(rq, false);
	}
}

void core_default_workqueue_base::process_request_response(request* r, bool synchronous)
{
	response* rp = process_request(r);

	ulocker_recursive scoped_lock(m_process_mutex);
	request_queue_type::iterator itr;
	for (itr = m_process_queue.begin(); itr != m_process_queue.end(); ++itr)
	{
		if ((*itr) == r)
		{
			m_process_queue.erase(itr);
			break;
		}
	}

	if (rp)
	{
		if (!rp->succeeded())
		{
			const request* req = rp->get_request();
			if (req->get_retry_count())
			{
				add_request_with_id(req->get_id(), req->get_channel(), req->get_type(), req->get_data(),
					req->get_retry_count() - 1);

				app_safe_delete(rp);
				return;
			}
		}

		if (synchronous)
		{
			process_response(rp);
			app_safe_delete(rp);
		}
		else
		{
			if (rp->get_request()->get_aborted())
			{
				rp->abort_request();
			}
			
			ulocker_recursive scoped_lock_rp(m_response_mutex);
			m_response_queue.push_back(rp);
		}

	}
	else
	{
		app_safe_delete(r);
	}
}

void core_default_workqueue_base::process_responses(void)
{
	int64 msStart = core_time::get_tick();
	int64 msCurrent = 0;

	while (true)
	{
		response* rp = 0;
		{
			ulocker_recursive scoped_lock_rp(m_response_mutex);
			if (m_response_queue.empty())
			{
				break;
			}
			else
			{
				rp = m_response_queue.front();
				m_response_queue.pop_front();
			}
		}

		if (rp)
		{
			process_response(rp);

			app_safe_delete(rp);

		}

		if (m_response_processing_timelimit)
		{
			msCurrent = core_time::get_tick();
			if (msCurrent - msStart > m_response_processing_timelimit)
				break;
		}
	}
}

core_workqueue::response* core_default_workqueue_base::process_request(request* r)
{
	request_handler_list_bychannel_type handler_list_copy;
	{
		ulocker_read scoped_lock(m_request_handler_mutex);
		handler_list_copy = m_request_handlers;

	}

	response* rp = 0;

	request_handler_list_bychannel_type::iterator itr = handler_list_copy.find(r->get_channel());
	if (itr != handler_list_copy.end())
	{
		request_handler_list_type& handlers = itr->second;
		for (request_handler_list_type::reverse_iterator jtr = handlers.rbegin(); jtr != handlers.rend(); ++jtr)
		{
			rp = (*jtr)->handle_request(r, this);

			if (rp)
				break;
		}
	}

	return rp;
}

void core_default_workqueue_base::process_response(response* r)
{
	response_handler_list_bychannel_type::iterator itr = m_response_handlers.find(r->get_request()->get_channel());
	if (itr != m_response_handlers.end())
	{
		response_handler_list_type& handlers = itr->second;
		for (response_handler_list_type::reverse_iterator jtr = handlers.rbegin(); jtr != handlers.rend(); ++jtr)
		{
			if ((*jtr)->can_handle_response(r, this))
			{
				(*jtr)->handle_response(r, this);
			}
		}
	}
}

core_default_workqueue::core_default_workqueue(const ustring& name /*= ""*/)
: core_default_workqueue_base(name)
{

}

core_default_workqueue::~core_default_workqueue(void)
{
	shutdown();
}

void core_default_workqueue::startup(int32 thread_num/* = 4*/, bool force_restart /*= true*/)
{
	if (m_is_running)
	{
		if (force_restart)
			shutdown();
		else
			return;
	}

	m_shuttingdown = false;

	for (int32 i = 0; i < thread_num; ++i)
	{
		core_thread* t = new core_thread(std::bind(&core_default_workqueue::_thread_main, this));
		t->start();
		m_thread_pool.push_back(t);
	}

	m_is_running = true;
}

void core_default_workqueue::shutdown()
{
	if (!m_is_running)
		return;

	m_shuttingdown = true;
	abort_all_requests();

	m_request_condition.notify_all();
	for (thread_pool_type::iterator itr = m_thread_pool.begin(); itr != m_thread_pool.end(); ++itr)
	{
		(*itr)->stop();
		app_safe_delete(*itr);
	}
	m_thread_pool.clear();

	m_is_running = false;
}

void core_default_workqueue::notify_workers()
{
	m_request_condition.notify_one();
}

void core_default_workqueue::wait_for_next_request()
{
	ulocker_recursive scoped_lock(m_request_mutex);
	if (m_request_queue.empty())
	{
		m_request_condition.wait(scoped_lock);
	}
}

void core_default_workqueue::_thread_main()
{
	while (!is_shuttingdown())
	{
		wait_for_next_request();
		_process_next_request();
	}
}

__END_NAMESPACE
