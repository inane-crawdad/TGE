#include "Common.h"

#ifdef PLATFORM_WINDOWS

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		
	}

	return TRUE;
}

#endif