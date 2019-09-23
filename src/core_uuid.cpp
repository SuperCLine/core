#include "stdafx.h"
#include "core_uuid.h"


__BEGIN_NAMESPACE

void core_uuid::init(int64 server_id /*= 1*/)
{
	m_id = util_clamp<int64>(server_id, 0, 1023); // 2^10 - 1

	m_base_time = core_time::get_unix(core_time::string_to_time_point("%4s-%2s-%2s %2s:%2s:%2s", "2019-09-09 05:02:09"));
	m_last_time = core_time::get_unix(core_time::now());
	m_concurrent_count = 0;
}

uint64 core_uuid::gen_uuid(void)
{
	static uint64 id_base = uint64(pow(2, 11) - 1);
	static uint64 concurrent_base = uint64(pow(2, 21) - 1);
	static uint64 second_base = uint64(pow(2, 35) - 1);

	int64 cur_time = core_time::get_unix(core_time::now());
	int64 second = cur_time - m_base_time;

	if (cur_time == m_base_time)
	{
		m_concurrent_count++;
	}
	else
	{
		m_concurrent_count = 0;
	}

	m_base_time = cur_time;

	return ((m_id & id_base) << 53) + ((m_concurrent_count & concurrent_base) << 33) + ((second & second_base));
}

__END_NAMESPACE
