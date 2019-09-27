#include "stdafx.h"
#include "core_log.h"

static interface_logmgr* g_core_logger = 0;
void set_core_logger(interface_logmgr* log)
{
	g_core_logger = log;
}

interface_logmgr* get_core_logger(void)
{
	return g_core_logger;
}

void core_log(ELogType type, const char* tag, const char* log)
{
	if (get_core_logger())
	{
		get_core_logger()->log(type, tag, log);
	}
}
