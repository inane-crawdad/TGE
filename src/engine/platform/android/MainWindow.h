#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "..\..\Common.h"

class CPlatformRender;

class CMainWindow : public IMainWindow, private CUncopyable
{
	CCore				*_pCore;
	android_app			*_app;
	bool				_bHasFocus;
	CPlatformRender		*_pPlatformRender;

	TProcDelegate		*_mLoopDel;
	TMsgProcDelegate	*_msgProcDel;

	static int _s_HandleInput(android_app* app, AInputEvent *event);
	static void _s_HandleCmd(android_app* app, int cmd);

	int _BeginMainLoop();

public:
	CMainWindow(CCore *pCore);
	~CMainWindow();

	HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) override final;
	HRESULT ConfigureWindow(const TWindowParams &winParams) override final;
	HRESULT SetCaption(const char *pCaption) override final;
	HRESULT GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) override final;
	HRESULT GetWindowHandle(WindowHandle &winHandle) override final;
	HRESULT GetWindowDrawContext(WindowDrawContext &dcHandle) override final;
	HRESULT BeginMainLoop() override final;
	HRESULT KillWindow() override final;
	HRESULT Free() override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};


#endif//_MAINWINDOW_H_
