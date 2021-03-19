#include "stdafx.h"
#include "core_time.h"


__BEGIN_NAMESPACE

int64 core_time::get_tick(void)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

int64 core_time::get_tick_nano(void)
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}


core_time::system_time_point_type core_time::now(void)
{
	return std::chrono::system_clock::now();
}

int64 core_time::get_unix(const system_time_point_type& tp)
{
	return std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
}

int64 core_time::get_unix_nano(const system_time_point_type& tp)
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
}

core_time::system_time_point_type& SuperCLine::core_time::add_hour(system_time_point_type& tp, int64 h)
{
	tp += std::chrono::hours(h);
	return tp;
}

core_time::system_time_point_type& core_time::add_minute(system_time_point_type& tp, int64 m)
{
	tp += std::chrono::minutes(m);
	return tp;
}

core_time::system_time_point_type& core_time::add_second(system_time_point_type& tp, int64 s)
{
	tp += std::chrono::seconds(s);
	return tp;
}

core_time::system_time_point_type& core_time::add_millsecond(system_time_point_type& tp, int64 ms)
{
	tp += std::chrono::milliseconds(ms);
	return tp;
}

core_time::system_time_point_type& core_time::add_microsecond(system_time_point_type& tp, int64 mms)
{
	tp += std::chrono::microseconds(mms);
	return tp;
}

void core_time::time_point_to_string(char* buf, std::size_t count, const char* format, const system_time_point_type& t)
{
	std::time_t tt = std::chrono::system_clock::to_time_t(t);
	calendar_time_to_string(buf, count, format, to_local_calendar_time(&tt));
}

core_time::system_time_point_type core_time::string_to_time_point(const char* buf, const char* format/* = "%d-%d-%d %d:%d:%d"*/)
{
	std::tm tm;
	app_memset(&tm, 0, sizeof(tm));
	string_to_calendar_time(&tm, format, buf);
	return core_time::system_time_point_type(std::chrono::system_clock::from_time_t(make_calendar_time(&tm)));
}



std::time_t core_time::get_calendar_time(void)
{
	return std::time(nullptr);
}

std::time_t core_time::make_calendar_time(std::tm* t)
{
	app_assert(t != NULL);

	return std::mktime(t);
}

std::tm* core_time::to_local_calendar_time(const std::time_t* t)
{
	return std::localtime(t);
}

std::tm* core_time::to_utc_calendar_time(const std::time_t* t)
{
	return std::gmtime(t);
}

void core_time::calendar_time_to_string(char* buf, std::size_t count, const char* format, const std::tm* t)
{
	if (!std::strftime(buf, count, format, t))
	{
		core_log(ELogType::Error, "core", "failed to format time!");
	}
}

void core_time::string_to_calendar_time(std::tm* t, const char* format, const char* buf)
{
	int32 year = 0;
	int32 month = 0;
	int32 day = 0;
	int32 hour = 0;
	int32 minute = 0;
	int32 second = 0;
	app_sscanf(buf, format, &year, &month, &day, &hour, &minute, &second);

	string_to_calendar_time(t, year, month, day, hour, minute, second);
}

void core_time::string_to_calendar_time(std::tm* t, int32 year, int32 month, int32 day, int32 hour /*= 0*/, int32 minute /*= 0*/, int32 second /*= 0*/)
{
	app_assert(t != NULL);

	t->tm_year = year - 1900;
	t->tm_mon = month - 1;
	t->tm_mday = day;
	t->tm_hour = hour;
	t->tm_min = minute;
	t->tm_sec = second;
}

const char* core_time::curtime_to_string(ETimeFlag flag/* = ETF_DOT*/)
{
	static char g_szTime[128] = { 0 };
	std::time_t tt = get_calendar_time();
	std::tm* t = to_local_calendar_time(&tt);

	switch (flag)
	{
	case ETimeFlag::ASC:
		app_strcpy(g_szTime, std::asctime(t));
		break;
	case ETimeFlag::DOT:
		calendar_time_to_string(g_szTime, 128, "%Y.%m.%d-%H.%M.%S", t);
		break;
	case ETimeFlag::COLON:
		calendar_time_to_string(g_szTime, 128, "%Y-%m-%d %H:%M:%S", t);
		break;
	case ETimeFlag::YMD:
		calendar_time_to_string(g_szTime, 128, "%Y-%m-%d", t);
		break;
	case ETimeFlag::HMS:
		calendar_time_to_string(g_szTime, 128, "%H-%M-%S", t);
		break;
	default:
		break;
	}

	return g_szTime;
}

__END_NAMESPACE
