#ifndef _INPUT_H_
#define _INPUT_H_

#include "Common.h"

class CInput : public IInput, private CUncopyable
{
	CCore			*_pCore;
	IPlatformInput	*_pPlatformInput;
	HWND			_hWnd;
	bool			_focused,
					_exclusive,
					_hideCursor,
					_curAlwaysInCenter,

					_keys[256];

	TMouseState		_mouseState;

	int				_centerX, _centerY; // center of client rect in client coord system

	int				_oldMouseX, _oldMouseY; // in client coord system

	void _ClipCursor();

	void _MsgProc(const TWindowMessage& msg);
	static void CALLBACK _s_MsgProc(const TWindowMessage& msg, void *pParam);

	void _Process();
	static void CALLBACK _s_Process(void *pParam);

public:

	CInput(CCore *pCore);
	~CInput();

	HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flags = ICF_DEFAULT) override final;
	HRESULT CALLBACK GetKey(E_KEYBOARD_KEY_CODES eKey, bool &isPressed) override final;
	HRESULT CALLBACK GetMouseState(TMouseState& mouseState) override final;

	HRESULT CALLBACK GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType) override final;
};

#endif