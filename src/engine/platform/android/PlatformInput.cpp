#include "PlatformInput.h"

CPlatformInput::CPlatformInput(CCore *pCore) :
_pCore(pCore)
{
	_pCore->AddToLog("Platform input subsystem was initialized!", false);
}
CPlatformInput::~CPlatformInput()
{
	_pCore->AddToLog("Platform input subsystem was finalized!", false);
}

HRESULT CPlatformInput::Free()
{
	delete this;
	return S_OK;
}

HRESULT CPlatformInput::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_INPUT;
	return S_OK;
}