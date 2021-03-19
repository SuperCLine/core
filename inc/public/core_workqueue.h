/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_workqueue.h
| AUTHOR     : https://supercline.com/
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-22      SuperCLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_WORKQUEUE_H_
#define _CORE_WORKQUEUE_H_

#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Winvalid-pch"
#endif

__BEGIN_NAMESPACE


class CORE_API core_workqueue
{
protected:
	typedef umap<ustring, uint16>			channel_map_type;
	typedef umap<ustring, uint16>::iterator channel_itr_type;

	uint16						m_next_channel;
	channel_map_type			m_channel_map;
	umutex_recursive			m_channel_map_mutex;

public:
	typedef unsigned long long int request_id_type;

	class CORE_API request
	{
		friend class core_workqueue;
	protected:
		uint16	m_channel;
		uint16	m_type;
		void*	m_data;
		uint8	m_retry_count;
		request_id_type	m_id;
		mutable bool m_aborted;

	public:
		request(uint16 channel, uint16 rtype, void* rdata, uint8 retry, request_id_type rid);
		~request(void);

		inline void abort_request(void) const { m_aborted = true; }
		inline uint16 get_channel(void) const { return m_channel; }
		inline uint16 get_type(void) const { return m_type; }
		inline void* get_data(void) const { return m_data; }
		inline uint8 get_retry_count(void) const { return m_retry_count; }
		inline request_id_type get_id(void) const { return m_id; }
		inline bool get_aborted(void) const { return m_aborted; }
	};

	struct CORE_API response
	{
		bool m_success;
		bool  m_own_data;
		const request* m_request;
		void* m_data;
		ustring m_messages;

	public:
		response(const request* rq, bool success, void* data, bool owndata = true, const ustring& msg = "");
		~response(void);

		inline const request* get_request(void) const { return m_request; }
		inline bool succeeded(void) const { return m_success; }
		inline const ustring& get_messages(void) const { return m_messages; }
		inline void* get_data(void) const { return m_data; }
		void abort_request(void);
	};

	class CORE_API request_handler
	{
	public:
		request_handler(void) {}
		virtual ~request_handler(void) {}

		virtual bool can_handle_request(const request* req, const core_workqueue* srcQ)
		{
			app_unreferenced(srcQ);
			return !req->get_aborted();
		}
		virtual response* handle_request(const request* req, const core_workqueue* srcQ) = 0;
	};

	class CORE_API response_handler
	{
	public:
		response_handler(void) {}
		virtual ~response_handler(void) {}

		virtual bool can_handle_response(const response* res, const core_workqueue* srcQ)
		{
			app_unreferenced(srcQ);
			return !res->get_request()->get_aborted();
		}
		virtual void handle_response(const response* res, const core_workqueue* srcQ) = 0;
	};

public:
	core_workqueue(void) : m_next_channel(0) {}
	virtual ~core_workqueue(void) {}

	virtual void startup(int32 thread_num = 4, bool force_restart = true) = 0;
	virtual void add_request_handler(uint16 channel, request_handler* rh) = 0;
	virtual void remove_request_handler(uint16 channel, request_handler* rh) = 0;
	virtual void add_response_handler(uint16 channel, response_handler* rh) = 0;
	virtual void remove_response_handler(uint16 channel, response_handler* rh) = 0;
	virtual request_id_type add_request(uint16 channel, uint16 rtype, void* rdata, uint8 retry_count = 0, bool force_synchronous = false) = 0;
	virtual void abort_request(request_id_type id) = 0;
	virtual void abort_requests_by_channel(uint16 channel) = 0;
	virtual void abort_all_requests(void) = 0;
	virtual void set_paused(bool pause) = 0;
	virtual bool is_paused(void) const = 0;
	virtual void set_requests_accepted(bool accept) = 0;
	virtual bool get_requests_accepted(void) const = 0;

	virtual void process_responses(void) = 0;

	virtual int64 get_response_processing_timelimit(void) const = 0;
	virtual void set_response_processing_timelimit(int64 ms) = 0;

	virtual void shutdown(void) = 0;

	virtual uint16 get_channel(const ustring& channel_name);
};

class CORE_API core_default_workqueue_base : public core_workqueue
{
public:
	core_default_workqueue_base(const ustring& name = "");
	virtual ~core_default_workqueue_base(void);

	inline const ustring& get_name(void) const { return m_name; }
	virtual bool is_shuttingdown() const { return m_shuttingdown; }

	virtual void _process_next_request();
	virtual void _thread_main() = 0;

	virtual void add_request_handler(uint16 channel, request_handler* rh);
	virtual void remove_request_handler(uint16 channel, request_handler* rh);
	virtual void add_response_handler(uint16 channel, response_handler* rh);
	virtual void remove_response_handler(uint16 channel, response_handler* rh);
	virtual request_id_type add_request(uint16 channel, uint16 rtype, void* rdata, uint8 retry_count = 0, bool force_synchronous = false);
	virtual void abort_request(request_id_type id);
	virtual void abort_requests_by_channel(uint16 channel);
	virtual void abort_all_requests(void);
	virtual void set_paused(bool pause);
	virtual bool is_paused(void) const { return m_paused; }
	virtual void set_requests_accepted(bool accept);
	virtual bool get_requests_accepted(void) const { return m_accept_requests; }

	virtual void process_responses(void);

	virtual int64 get_response_processing_timelimit(void) const { return m_response_processing_timelimit; }
	virtual void set_response_processing_timelimit(int64 ms) { m_response_processing_timelimit = ms; }

protected:
	ustring			m_name;
	bool			m_is_running;
	int64			m_response_processing_timelimit; //ms

	typedef udeque<request*>	request_queue_type;
	typedef udeque<response*>	response_queue_type;

	request_queue_type	m_request_queue;
	request_queue_type	m_process_queue;
	response_queue_type	m_response_queue;

	class request_handler_holder
	{
	protected:
		umutex_rdwt			m_mutex_rdwt;
		request_handler*	m_handler;

	public:
		request_handler_holder(request_handler* handler) 
			: m_handler(handler) 
		{}

		void disconnect_handler()
		{
			ulocker_write scoped_lock(m_mutex_rdwt);
			m_handler = 0;
		}

		inline request_handler* get_handler(void) { return m_handler; }

		response* handle_request(const request* req, const core_workqueue* srcQ)
		{
			ulocker_read scoped_lock(m_mutex_rdwt);
			response* r = 0;
			if (m_handler)
			{
				if (m_handler->can_handle_request(req, srcQ))
				{
					r = m_handler->handle_request(req, srcQ);
				}
			}
			return r;
		}
	};

	typedef ushared_ptr<request_handler_holder>			request_handler_holder_ptr_type;
	typedef ulist<request_handler_holder_ptr_type>		request_handler_list_type;
	typedef ulist<response_handler*>					response_handler_list_type;
	typedef umap<uint16, request_handler_list_type>		request_handler_list_bychannel_type;
	typedef umap<uint16, response_handler_list_type>	response_handler_list_bychannel_type;

	request_handler_list_bychannel_type		m_request_handlers;
	response_handler_list_bychannel_type	m_response_handlers;
	request_id_type	m_request_count;
	bool		m_paused;
	bool		m_accept_requests;
	bool		m_shuttingdown;

	umutex_recursive m_request_mutex;
	umutex_recursive m_process_mutex;
	umutex_recursive m_response_mutex;
	umutex_rdwt m_request_handler_mutex;

	void process_request_response(request* r, bool synchronous);
	response* process_request(request* r);
	void process_response(response* r);

	virtual void notify_workers() = 0;

	void add_request_with_id(request_id_type rid, uint16 channel, uint16 rtype, void* rdata, uint8 retry_count);
};

class CORE_API core_default_workqueue : public core_default_workqueue_base
{
public:
	core_default_workqueue(const ustring& name = "");
	virtual ~core_default_workqueue(void);

	virtual void _thread_main();
	virtual void shutdown();
	virtual void startup(int32 thread_num = 4, bool force_restart = true);

protected:
	virtual void wait_for_next_request();
	virtual void notify_workers();

	std::condition_variable_any m_request_condition;

	typedef uvector<core_thread*>	thread_pool_type;
	thread_pool_type m_thread_pool;
};

__END_NAMESPACE

#if defined(_MSC_VER)
#pragma warning(default : 4251)
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#endif //_CORE_WORKQUEUE_H_