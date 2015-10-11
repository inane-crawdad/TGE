#ifndef _PLATFORMSUBSYSTEMMANAGER_H_
#define _PLATFORMSUBSYSTEMMANAGER_H_

#include "PlatformAPI.h"

class CPlatformSubsystemManager :private TGE::CUncopyable
{
	CPlatformSubsystemManager(){}

public:
	static CPlatformSubsystemManager& Instance()
	{
		static CPlatformSubsystemManager instance;
		return instance;
	}

	HRESULT GetPlatformSubsystem(TGE::ICore *pCore, E_PLATFORM_SUB_SYSTEM_TYPE type, IPlatformSubsystem *&pPSS);
};


#endif