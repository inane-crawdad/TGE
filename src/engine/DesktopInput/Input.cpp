#include "Input.h"

CInput::CInput(CCore *pCore):
_pCore(pCore),
_focused(true),
_exclusive(false),
_hideCursor(false),
_curAlwaysInCenter(false),
_centerX(0),
_centerY(0),
_oldMouseX(0),
_oldMouseY(0)
{
	CPlatformSubsystemManager::Instance().GetPlatformSubsystem(
		_pCore,
		PSST_INPUT,
		(IPlatformSubsystem*&)_pPlatformInput);

	_mouseState.X = 0;
	_mouseState.Y = 0;
	_mouseState.deltaX = 0;
	_mouseState.deltaY = 0;
	_mouseState.deltaWheel = 0;
	_mouseState.lButtonPressed = false;
	_mouseState.rButtonPressed = false;
	_mouseState.mButtonPressed = false;

	memset(_keys, 0, 256);

	_pCore->pDMessageProc()->Add(_s_MsgProc, this);
	_pCore->AddFunction(FT_PROCESS, _s_Process, this);

	_pPlatformInput->GetCursorPos(_oldMouseX, _oldMouseY);

	_pCore->AddToLog("Input subsystem was initialized!", false);
}
CInput::~CInput()
{
	if (_exclusive)
		_pPlatformInput->ClipCursor(0, 0, 0, 0);

	_pCore->pDMessageProc()->Remove(_s_MsgProc, this);
	_pCore->RemoveFunction(FT_PROCESS, _s_Process, this);

	_pCore->AddToLog("Input subsystem was finalized!", false);
}

HRESULT CInput::Configure(E_INPUT_CONFIGURATION_FLAGS flags)
{
	_exclusive = (flags & ICF_EXCLUSIVE) != 0;
	_hideCursor = (flags & ICF_HIDE_CURSOR) != 0;
	_curAlwaysInCenter = (flags & ICF_ALWAYS_IN_CENTER) != 0;

	if (_exclusive)
		_ClipCursor();
	else
		_pPlatformInput->ClipCursor(0, 0, 0, 0);

	if (_hideCursor)
		_pPlatformInput->ShowCursor(false);
	else
		_pPlatformInput->ShowCursor(true);

	if (_curAlwaysInCenter)
	{
		_pPlatformInput->GetCenter(_centerX, _centerY);
		_pPlatformInput->SetCursorPos(_centerX, _centerY);
	}
		

	return S_OK;
}

HRESULT CInput::GetKey(E_KEYBOARD_KEY_CODES eKey, bool &isPressed)
{
	isPressed = _keys[eKey];
	return S_OK;
}

HRESULT CInput::GetMouseState(TMouseState& mouseState)
{
	mouseState = _mouseState;
	return S_OK;
}

void CInput::_ClipCursor()
{
	int32 left, right, top, bottom;
	_pCore->GetMainWindow()->GetClientRect(left, right, top, bottom);
	_pPlatformInput->ClipCursor(left, right, top, bottom);
}

void CInput::_s_MsgProc(const TWindowMessage& msg, void *pParam)
{
	((CInput*)pParam)->_MsgProc(msg);
}

void CInput::_MsgProc(const TWindowMessage& msg)
{
	switch (msg.messageType)
	{
	case(WMT_ACTIVATED) :
		if (_focused)
			break;
		_focused = true;
		if (_exclusive)
			_ClipCursor();

		break;
	
	case(WMT_DEACTIVATED) :
		if (!_focused)
			break;
		_focused = false;
		if (_exclusive)
			_pPlatformInput->ClipCursor(0, 0, 0, 0);

		memset(_keys, 0, 256);
		_mouseState.lButtonPressed = false;
		_mouseState.rButtonPressed = false;
		_mouseState.mButtonPressed = false;

		break;
	
	case(WMT_MOVE) :
		if (_curAlwaysInCenter)
			_pPlatformInput->GetCenter(_centerX, _centerY);
		break;
	
	case(WMT_KEY_DOWN) :
		_keys[msg.param1] = true;
		break;
	
	case(WMT_KEY_UP) :
		_keys[msg.param1] = false;
		break;
	
	case(WMT_MOUSE_DOWN) :
		if (msg.param1 == 0)
			_mouseState.lButtonPressed = true;
		else
			if (msg.param1 == 1)
				_mouseState.rButtonPressed = true;
			else
				_mouseState.mButtonPressed = true;
		break;
	
	case(WMT_MOUSE_UP) :
		if (msg.param1 == 0)
			_mouseState.lButtonPressed = false;
		else
			if (msg.param1 == 1)
				_mouseState.rButtonPressed = false;
			else
				_mouseState.mButtonPressed = false;
		break;
	
	case(WMT_MOUSE_WHEEL) :
		_mouseState.deltaWheel = msg.param1;
		break;
	case(WMT_MOUSE_MOVE) :
		_mouseState.X = msg.param1;
		_mouseState.Y = msg.param2;
		break;
	default:
		break;
	}
}

void CInput::_Process()
{
	if (_curAlwaysInCenter)
	{
		_mouseState.deltaX = _mouseState.X - _centerX;
		_mouseState.deltaY = _mouseState.Y - _centerY;
		_mouseState.X = _centerX;
		_mouseState.Y = _centerY;

		_pPlatformInput->SetCursorPos(_centerX, _centerY);
	}
	else
	{
		_mouseState.deltaX = _mouseState.X - _oldMouseX;
		_mouseState.deltaY = _mouseState.Y - _oldMouseY;
		_oldMouseX = _mouseState.X;
		_oldMouseY = _mouseState.Y;
	}
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