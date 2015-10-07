#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "..\..\Common.h"

class CMainWindow : public IMainWindow, private CUncopyable
{
	ICore				*_pCore;

	HINSTANCE			_hInst;
	HWND				_hWnd;
	HDC					_hDC;
	HGLRC				_hGLRC;
	uint				_msaaSamples;


	TProcDelegate		*_mLoopDel;
	TMsgProcDelegate	*_msgProcDel;

	static LRESULT CALLBACK _s_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int CMainWindow::_BeginMainLoop();

	CMainWindow(const CMainWindow&);
	CMainWindow& operator=(const CMainWindow&);
public:
	CMainWindow(ICore *pCore);
	~CMainWindow();

	HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) override final;
	HRESULT ConfigureWindow(uint resX, uint resY, bool fullScreen) override final;
	HRESULT SetCaption(const char *pCaption) override final;
	HRESULT GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) override final;
	HRESULT GetWindowHandle(WindowHandle &winHandle) override final;
	HRESULT BeginMainLoop() override final;
	HRESULT KillWindow() override final;
	HRESULT Free() override final;
};


#endif//_MAINWINDOW_H_
