#include "Input.h"

CInput::CInput(CCore *pCore):
_pCore(pCore),
_exclusive(false),
_hideCursor(false),
_curAlwaysInCenter(false)
{
	_mouseState.X = 0;
	_mouseState.Y = 0;
	_mouseState.deltaX = 0;
	_mouseState.deltaY = 0;
	_mouseState.deltaWheel = 0;
	_mouseState.lButtonPressed = false;
	_mouseState.rButtonPressed = false;
	_mouseState.mButtonPressed = false;

	ZeroMemory(_keys, 256);

	_pCore->pDMessageProc()->Add(_s_MsgProc, this);

	_pCore->AddToLog("Input subsystem was initialized!", false);
}
CInput::~CInput()
{
	if (_exclusive)
		ClipCursor(NULL);

	_pCore->pDMessageProc()->Remove(_s_MsgProc, this);

	_pCore->AddToLog("Input subsystem was finalized!", false);
}

HRESULT CInput::Configure(E_INPUT_CONFIGURATION_FLAGS flags)
{
	_exclusive = (flags & E_INPUT_CONFIGURATION_FLAGS::ICF_EXCLUSIVE) != 0;
	_hideCursor = (flags & E_INPUT_CONFIGURATION_FLAGS::ICF_HIDE_CURSOR) != 0;
	_curAlwaysInCenter = (flags & E_INPUT_CONFIGURATION_FLAGS::ICF_ALWAYS_IN_CENTER) != 0;

	if (_exclusive)
		_ClipCursor();
	else
		ClipCursor(NULL);

	if (_hideCursor)
		ShowCursor(FALSE);
	else
		ShowCursor(TRUE);

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
	RECT rc;
	_pCore->GetMainWindow()->GetClientRect(rc.left, rc.right, rc.top, rc.bottom);
	ClipCursor(&rc);
}


void CInput::_s_MsgProc(const TGE::TWindowMessage& msg, void *pParam)
{
	((CInput*)pParam)->_MsgProc(msg);
}

void CInput::_MsgProc(const TGE::TWindowMessage& msg)
{
	switch (msg.messageType)
	{
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
		if (_curAlwaysInCenter)
		{
			_mouseState.deltaX = msg.param1 - _mouseState.X;
			_mouseState.deltaY = msg.param2 - _mouseState.Y;
		}
		_mouseState.X = msg.param1;
		_mouseState.Y = msg.param2;
		break;
	default:
		break;
	}
}