#include "stdafx.h"
#include "core_dumper.h"

#if defined(WIN32) || defined(_WIN32)
#include "dbghelp.h"

// based on dbghelp.h
typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

static char g_dump_file[app_max_path * 2] = "";
LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	char szDbgHelpPath[app_max_path] = { 0 };

	if (GetModuleFileName(NULL, szDbgHelpPath, app_max_path))
	{
		char *p = app_strrchr(szDbgHelpPath, '\\');
		if (p)
		{
			_tcscpy(p + 1, "DBGHELP.DLL");
			hDll = ::LoadLibrary(szDbgHelpPath);
		}
	}

	if (hDll == NULL)
	{
		// load any version we can
		hDll = ::LoadLibrary("DBGHELP.DLL");
	}

	LPCTSTR szResult = NULL;
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
		if (pDump)
		{
			char szScratch[app_max_path] = { 0 };
			HANDLE hFile = ::CreateFile(g_dump_file, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

				ExInfo.ThreadId = ::GetCurrentThreadId();
				ExInfo.ExceptionPointers = pExceptionInfo;
				ExInfo.ClientPointers = NULL;

				// write the dump
				BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
				if (bOK)
				{
					sprintf(szScratch, "succeed to save dump file ('%s')", g_dump_file);
					szResult = szScratch;
					retval = EXCEPTION_EXECUTE_HANDLER;
				}
				else
				{
					sprintf(szScratch, "failed to save dump file ('%s')-(error:%d)", g_dump_file, GetLastError());
					szResult = szScratch;
				}
				::CloseHandle(hFile);
			}
			else
			{
				sprintf(szScratch, "failed to create dump file ('%s')-(error:%d)", g_dump_file, GetLastError());
				szResult = szScratch;
			}
		}
		else
		{
			szResult = "DBGHELP.DLL too old";
		}
	}
	else
	{
		szResult = "DBGHELP.DLL not found";
	}

	return retval;
}
#endif


__BEGIN_NAMESPACE

core_dumper::core_dumper(const char* dump_path, const char* dump_file)
{
	app_assert(dump_path != NULL && dump_file != NULL);

#if defined(WIN32) || defined(_WIN32)
	app_strcat(g_dump_file, dump_path);
	app_strcat(g_dump_file, "\\dump\\");
	if (-1 == app_file_exist(g_dump_file, 0))
	{
		if (!::CreateDirectory(g_dump_file, NULL))
		{
			core_logf(ELT_ERROR, "core", "failed to create dump directory \'%s\'!", g_dump_file);
			return;
		}
	}

	app_strcat(g_dump_file, dump_file);
	app_strcat(g_dump_file, "_");
	app_strcat(g_dump_file, core_time::curtime_to_string());
	app_strcat(g_dump_file, ".dmp");

	::SetUnhandledExceptionFilter(TopLevelFilter);
#endif
}

__END_NAMESPACE