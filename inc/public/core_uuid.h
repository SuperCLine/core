/*------------------------------------------------------------------------------
|
| COPYRIGHT (C) 2018 - 2026 All Right Reserved
|
| FILE NAME  : \core\inc\public\core_uuid.h
| AUTHOR     : CLine
| PURPOSE    :
|
| SPEC       :
|
| MODIFICATION HISTORY
|
| Ver      Date            By              Details
| -----    -----------    -------------   ----------------------
| 1.0      2019-9-22      CLine           Created
|
+-----------------------------------------------------------------------------*/


#ifndef _CORE_UUID_H_
#define _CORE_UUID_H_


__BEGIN_NAMESPACE


class CORE_API core_uuid
{
public:
	core_uuid(void) : m_id (0)
	{}

	~core_uuid(void)
	{}

	void init(int64 server_id = 1);

	// 2^10 server num, 2^20 concurrent per second, 2^34 seconds base on "2019-09-09 05:02:09"
	uint64 gen_uuid(void);

	static ustring to_string(uint64 id);

private:
	int64 m_id;
	int64 m_base_time;
	int64 m_last_time;
	int64 m_concurrent_count;
};


__END_NAMESPACE


#endif //_CORE_UUID_H_