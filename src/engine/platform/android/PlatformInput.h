#ifndef _PLATFORMINPUT_H_
#define _PLATFORMINPUT_H_

#include "..\..\Common.h"

class CPlatformInput : public IPlatformInput, private CUncopyable
{
	CCore		*_pCore;

public:
	CPlatformInput(CCore *pCore);
	~CPlatformInput();

	HRESULT Free() override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};


#endif