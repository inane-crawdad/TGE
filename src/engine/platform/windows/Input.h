#ifndef _INPUT_H_
#define _INPUT_H_

#include "..\..\Common.h"
#include "..\..\Core.h"

class CCore;

class CInput : public IInput, private CUncopyable
{
	CCore			*_pCore;
	HWND			_hWnd;
	bool			_focused,
					_exclusive,
					_hideCursor,
					_curAlwaysInCenter,

					_keys[256];

	TMouseState		_mouseState;

	int				_centerX,
					_centerY;

	int				_oldMouseX, _oldMouseY;

	void _ClipCursor();
	void _UpdateCenterCoord();

	void _MsgProc(const TWindowMessage& msg);
	static void CALLBACK _s_MsgProc(const TWindowMessage& msg, void *pParam);

	void _Process();
	static void CALLBACK _s_Process(void *pParam);

	CInput(const CInput&);
	CInput& operator=(const CInput&);

public:

	CInput(CCore *pCore);
	~CInput();

	HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flags = ICF_DEFAULT) override final;
	HRESULT CALLBACK GetKey(E_KEYBOARD_KEY_CODES eKey, bool &isPressed) override final;
	HRESULT CALLBACK GetMouseState(TMouseState& mouseState) override final;
};

#endif