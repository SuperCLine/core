#include "stdafx.h"

__BEGIN_NAMESPACE

static interface_logmgr* g_logger = NULL;
CORE_API void init_core(interface_logmgr* log)
{
	g_logger = log;
}

CORE_API interface_logmgr* get_logger(void)
{
	return g_logger;
}

CORE_API void core_log(ELogType type, const char* tag, const char* log)
{
	if (g_logger)
	{
		g_logger->log(type, tag, log);
	}
}

__END_NAMESPACE
