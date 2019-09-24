#include "stdafx.h"
#include "core_uuid.h"


__BEGIN_NAMESPACE

void core_uuid::init(int64 server_id /*= 1*/)
{
	m_id = util_clamp<int64>(server_id, 0, 1023); // 2^10 - 1

	m_base_time = core_time::get_unix(core_time::string_to_time_point("2019-09-09 05:02:09"));
	m_last_time = core_time::get_unix(core_time::now());
	m_concurrent_count = 0;
}

uint64 core_uuid::gen_uuid(void)
{
	static uint64 id_base = 0x3FF;
	static uint64 concurrent_base = 0xFFFFF;
	static uint64 second_base = 0x3FFFFFFFF;

	int64 cur_time = core_time::get_unix(core_time::now());
	int64 second = cur_time - m_base_time;

	if (cur_time == m_last_time)
	{
		m_concurrent_count++;
	}
	else
	{
		m_concurrent_count = 0;
	}

	m_last_time = cur_time;

	return ((m_id & id_base) << 54) + ((m_concurrent_count & concurrent_base) << 34) + ((second & second_base));
}

ustring core_uuid::to_string(uint64 id)
{
	return util_to_hex<uint64>(id);
}

__END_NAMESPACE
