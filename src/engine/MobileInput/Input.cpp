#include "Input.h"

CInput::CInput(CCore *pCore) :
_pCore(pCore)
{
	CPlatformSubsystemManager::Instance().GetPlatformSubsystem(
		_pCore,
		PSST_INPUT,
		(IPlatformSubsystem*&)_pPlatformInput);

	_pCore->AddToLog("Input subsystem was initialized!", false);
}
CInput::~CInput()
{

	_pCore->AddToLog("Input subsystem was finalized!", false);
}

HRESULT CInput::GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType)
{
	subsystemType = ESS_INPUT;
	return S_OK;
}