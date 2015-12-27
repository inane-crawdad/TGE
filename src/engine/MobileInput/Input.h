#ifndef _INPUT_H_
#define _INPUT_H_

#include "../Common.h"

class CInput : public IInput, private CUncopyable
{
	CCore			*_pCore;
	IPlatformInput	*_pPlatformInput;

public:

	CInput(CCore *pCore);
	~CInput();

	HRESULT CALLBACK GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType) override final;
};

#endif