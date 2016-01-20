#include "Input.h"

CInput::CInput(CCore *pCore) :
_pCore(pCore),
_pTapDetector(nullptr),
_pDoubleTapDetector(nullptr),
_pDragDetector(nullptr),
_pPinchDetector(nullptr)
{
	CPlatformSubsystemManager::Instance().GetPlatformSubsystem(
		_pCore,
		PSST_INPUT,
		(IPlatformSubsystem*&)_pPlatformInput);

	_pCore->pDMessageProc()->Add(_s_MsgProc, this);
	_pCore->AddFunction(FT_PROCESS, _s_Process, this);

	_pCore->AddToLog("Input subsystem was initialized!", false);
}
CInput::~CInput()
{
	_ClearDetectors();

	_pCore->pDMessageProc()->Remove(_s_MsgProc, this);
	_pCore->RemoveFunction(FT_PROCESS, _s_Process, this);

	_pCore->AddToLog("Input subsystem was finalized!", false);
}

void CInput::_ClearDetectors()
{
	_pTapDetector = nullptr;
	_pDoubleTapDetector = nullptr;
	_pDragDetector = nullptr;
	_pPinchDetector = nullptr;
}

HRESULT CInput::Configure(E_GESTURE_TYPE flags, const TTouchParams& touchParams)
{
	_pPlatformInput->Configure(touchParams);

	_ClearDetectors();

	if (flags & GT_TAPS)
		_pPlatformInput->GetGestureDetector(GDT_TAP, _pTapDetector);

	if (flags & GT_DOUBLE_TAPS)
		_pPlatformInput->GetGestureDetector(GDT_DOUBLE_TAP, _pDoubleTapDetector);

	if (flags & GT_DRAGS)
		_pPlatformInput->GetGestureDetector(GDT_DRAG, _pDragDetector);

	if (flags & GT_PINCH)
		_pPlatformInput->GetGestureDetector(GDT_PINCH, _pPinchDetector);

	return S_OK;
}
HRESULT CInput::GetGestureState(E_GESTURE_TYPE gestureType, TGestureState& gestureState)
{
	switch (gestureType)
	{
	case GT_TAPS:
		if (_pTapDetector != nullptr)
			_pTapDetector->GetGestureState(gestureState);
		break;
	case GT_DOUBLE_TAPS:
		if (_pDoubleTapDetector != nullptr)
			_pDoubleTapDetector->GetGestureState(gestureState);
		break;
	case GT_DRAGS:
		if (_pDragDetector != nullptr)
			_pDragDetector->GetGestureState(gestureState);
		break;
	case GT_PINCH:
		if (_pPinchDetector != nullptr)
			_pPinchDetector->GetGestureState(gestureState);
		break;
	default:
		return E_INVALIDARG;
		break;

	}

	return S_OK;
}

void CInput::_s_MsgProc(const TWindowMessage& msg, void *pParam)
{
	((CInput*)pParam)->_MsgProc(msg);
}

void CInput::_MsgProc(const TWindowMessage& msg)
{
	switch (msg.messageType)
	{
	case WMT_TOUCH_EVENT:
		if (_pTapDetector != nullptr)
			_pTapDetector->Detect(msg);
		
		if (_pDoubleTapDetector != nullptr)
			_pDoubleTapDetector->Detect(msg);
		
		if (_pDragDetector != nullptr)
			_pDragDetector->Detect(msg);
		
		if (_pPinchDetector != nullptr)
			_pPinchDetector->Detect(msg);
		break;
	default:
		break;
	}
}

void CInput::_Process()
{
	if (_pTapDetector != nullptr)
		_pTapDetector->UpdateState();

	if (_pDoubleTapDetector != nullptr)
		_pDoubleTapDetector->UpdateState();

	if (_pDragDetector != nullptr)
		_pDragDetector->UpdateState();

	if (_pPinchDetector != nullptr)
		_pPinchDetector->UpdateState();
}

void CInput::_s_Process(void *pParam)
{
	((CInput*)pParam)->_Process();
}

HRESULT CInput::GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType)
{
	subsystemType = ESS_INPUT;
	return S_OK;
}