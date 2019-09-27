#include "stdafx.h"

__BEGIN_NAMESPACE

CORE_API void load_module_core(interface_logmgr* log)
{
	set_core_logger(log);
}

CORE_API void unload_module_core(void)
{
	set_core_logger(0);
}

__END_NAMESPACE
