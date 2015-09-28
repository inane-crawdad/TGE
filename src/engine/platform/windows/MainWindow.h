#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "..\..\Common.h"

class CMainWindow : public IMainWindow
{
	ICore				*_pCore;

	HINSTANCE			_hInst;
	HWND				_hWnd;
	HDC					_hDC;
	HGLRC				_hGLRC;
	uint				_msaaSamples;


	TProcDelegate		*_procDel;
	TMsgProcDelegate	*_msgProcDel;

	static LRESULT CALLBACK _s_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int CMainWindow::_BeginMainLoop();
public:
	CMainWindow(ICore *pCore);
	~CMainWindow();

	HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate);
	HRESULT ConfigureWindow(uint resX, uint resY, bool fullScreen);
	HRESULT SetCaption(const char *pCaption);
	HRESULT BeginMainLoop();
	HRESULT KillWindow();
	HRESULT Free();
};


#endif
