#ifndef _PLATFORMINPUT_H_
#define _PLATFORMINPUT_H_

#include "..\..\Common.h"

class CPlatformInput : public IPlatformInput, private CUncopyable
{
	CCore		*_pCore;
	HWND		_hWnd;
	HCURSOR		_blankCursor;

public:
	CPlatformInput(CCore *pCore);
	~CPlatformInput();

	HRESULT GetCursorPos(int &x, int &y) override final;
	HRESULT SetCursorPos(int x, int y) override final;
	HRESULT GetCenter(int &x, int &y) override final;
	HRESULT ShowCursor(bool show) override final;
	HRESULT ClipCursor(TGE::int32 left, TGE::int32 right, TGE::int32 top, TGE::int32 bottom) override final;
	HRESULT Free() override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};


#endif