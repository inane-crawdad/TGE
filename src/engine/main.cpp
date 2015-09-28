#include "Common.h"

//temp
#include <iostream>
//


#ifdef PLATFORM_WINDOWS

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		std::cout << "TGE is attached" << std::endl;
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		std::cout << "TGE is detached" << std::endl;
	}

	return TRUE;
}

#endif