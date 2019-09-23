/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_time.h
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


#ifndef _CORE_TIME_H_
#define _CORE_TIME_H_


__BEGIN_NAMESPACE

enum ETimeFlag
{
	ETF_ASC = 0,	//Www Mmm dd hh:mm:ss yyyy
	ETF_DOT,		//yyyy.MM.dd-hh.mm.ss
	ETF_COLON,		//yyyy-MM-dd hh:mm:ss
	ETF_YMD,		//yyyy-MM-dd
	ETF_HMS,		//hh-mm-ss
};

class CORE_API core_time
{
public:
	typedef std::chrono::steady_clock						steady_clock_type;
	typedef typename std::chrono::steady_clock::duration	steady_duration_type;
	typedef typename std::chrono::steady_clock::time_point	steady_time_point_type;

	typedef std::chrono::system_clock						system_clock_type;
	typedef typename std::chrono::system_clock::duration	system_duration_type;
	typedef typename std::chrono::system_clock::time_point	system_time_point_type;

public:
	// from steady_clock(ms)
	static int64 get_tick(void);
	// from steady_clock(ns)
	static int64 get_tick_nano(void);

	// from system_clock
	static system_time_point_type now(void);

	// from system_clock to get time stamp (s) since January 1, 1970 UTC
	static int64 get_unix(const system_time_point_type& tp);
	
	// from system_clock to get time stamp (ns) since January 1, 1970 UTC
	static int64 get_unix_nano(const system_time_point_type& tp);

	static system_time_point_type& add_hour(system_time_point_type& tp, int64 h);
	static system_time_point_type& add_minute(system_time_point_type& tp, int64 m);
	static system_time_point_type& add_second(system_time_point_type& tp, int64 s);
	static system_time_point_type& add_millsecond(system_time_point_type& tp, int64 ms);

	static void time_point_to_string(char* buf, std::size_t count, const char* format, const system_time_point_type& t);
	static system_time_point_type string_to_time_point(const char* format, const char* buf);

	// calendar time
	static std::time_t get_calendar_time(void);
	static std::time_t make_calendar_time(std::tm* t);
	static std::tm* to_local_calendar_time(const std::time_t* t);
	static std::tm* to_utc_calendar_time(const std::time_t* t);
	
	static void calendar_time_to_string(char* buf, std::size_t count, const char* format, const std::tm* t);
	
	// format: YYYY-mm-dd HH:MM:SS
	static void string_to_calendar_time(std::tm* t, const char* format, const char* buf);
	static void string_to_calendar_time(std::tm* t, int32 year, int32 month, int32 day, int32 hour = 0, int32 minute = 0, int32 second = 0);

	static const char* curtime_to_string(ETimeFlag flag = ETF_DOT);
};

__END_NAMESPACE


#endif //_CORE_TIME_H_