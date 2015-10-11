#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "..\..\Common.h"

class CMainWindow : public IMainWindow, private CUncopyable
{
	CCore				*_pCore;

	HINSTANCE			_hInst;
	HWND				_hWnd;
	HDC					_hDC;
	HGLRC				_hGLRC;
	uint				_msaaSamples;


	TProcDelegate		*_mLoopDel;
	TMsgProcDelegate	*_msgProcDel;

	static LRESULT CALLBACK _s_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int CMainWindow::_BeginMainLoop();

public:
	CMainWindow(CCore *pCore);
	~CMainWindow();

	HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) override final;
	HRESULT ConfigureWindow(const TGE::TWindowParams &winParams) override final;
	HRESULT SetCaption(const char *pCaption) override final;
	HRESULT GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) override final;
	HRESULT GetWindowHandle(WindowHandle &winHandle) override final;
	HRESULT GetWindowDrawContext(TGE::WindowDrawContext &dcHandle) override final;
	HRESULT BeginMainLoop() override final;
	HRESULT KillWindow() override final;
	HRESULT Free() override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};


#endif//_MAINWINDOW_H_
