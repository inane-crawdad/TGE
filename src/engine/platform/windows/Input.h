#ifndef _INPUT_H_
#define _INPUT_H_

#include "..\..\Common.h"
#include "..\..\Core.h"

class CCore;

class CInput : public IInput
{
	CCore			*_pCore;

	bool			_exclusive;
	bool			_hideCursor;
	bool			_curAlwaysInCenter;

	bool			_keys[256];

	TMouseState		_mouseState;

	void _ClipCursor();

	void _MsgProc(const TGE::TWindowMessage& msg);
	static void CALLBACK _s_MsgProc(const TGE::TWindowMessage& msg, void *pParam);

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