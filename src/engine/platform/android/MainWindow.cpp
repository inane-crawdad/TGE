#include "MainWindow.h"
#include "PlatformRender.h"

CMainWindow::CMainWindow(CCore *pCore) :
_pCore(pCore),
_app(nullptr),
_bHasFocus(false),
_pPlatformRender(nullptr)
{}

CMainWindow::~CMainWindow()
{}

int CMainWindow::_s_HandleInput(android_app* app, AInputEvent *event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		CMainWindow *this_ptr = static_cast<CMainWindow*>(app->userData);
		this_ptr->_msgProcDel->Invoke(AndroidMsgToEngMsg(AET_INPUT, event, 0));
		return 1;
	}
	
	return 0;
}
void CMainWindow::_s_HandleCmd(android_app* app, int cmd)
{
	CMainWindow *this_ptr = static_cast<CMainWindow*>(app->userData);
	switch (cmd)
	{
	case APP_CMD_GAINED_FOCUS:
		this_ptr->_bHasFocus = true;
		break;
	case APP_CMD_LOST_FOCUS:
		this_ptr->_bHasFocus = false;
		break;
	case APP_CMD_CONFIG_CHANGED:
		if (this_ptr->_pPlatformRender->IsSizeChanged())
		{
			EGLint width = 0, height = 0;
			this_ptr->_pPlatformRender->GetWindowSize(width, height);
			this_ptr->_msgProcDel->Invoke(TGE::TWindowMessage(WMT_RESIZE, width, height));
		}	
		break;
	}

	this_ptr->_msgProcDel->Invoke(AndroidMsgToEngMsg(AET_CMD, nullptr, cmd));
}

int CMainWindow::_BeginMainLoop()
{
	CPlatformSubsystemManager::Instance().GetPlatformSubsystem(
		_pCore,
		PSST_RENDER,
		(IPlatformSubsystem*&)_pPlatformRender);

	_pCore->AddToLog("*** Begin main loop ***", false);
	while (true)
	{
		int id;
		int events;
		android_poll_source *source;

		while ((id = ALooper_pollAll(_bHasFocus ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0)
		{
			if (source != NULL)
				source->process(_app, source);

			if (_app->destroyRequested != 0)
			{
				_pCore->AddToLog("*** Exit main loop ***", false);
				_mLoopDel->Invoke();
				return 0;
			}
		}

		if (_bHasFocus)
		{
			_mLoopDel->Invoke();
			_pPlatformRender->SwapBuffers();
		}
	}
}

HRESULT CMainWindow::InitWindow(TProcDelegate *mLoopDelegate, TMsgProcDelegate *msgProcDelegate)
{
	_mLoopDel = mLoopDelegate;
	_msgProcDel = msgProcDelegate;
	
	return S_OK;
}

HRESULT CMainWindow::ConfigureWindow(const TGE::TWindowParams &winParams)
{
	if (winParams.pParam == nullptr)
	{
		_pCore->AddToLog("There is no android_app handle", false);
		return E_INVALIDARG;
	}

	_pCore->AddToLog("Configuring window", false);
	_app = static_cast<android_app*>(winParams.pParam);
	_app->userData = this;
	_app->onAppCmd = _s_HandleCmd;
	_app->onInputEvent = _s_HandleInput;

	return S_OK;
}

HRESULT CMainWindow::SetCaption(const char *pCaption)
{
	return S_OK;
}

HRESULT CMainWindow::GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom)
{
	EGLint width = 0, height = 0;
	_pPlatformRender->GetWindowSize(width, height);
	
	if (width == 0 && height == 0)
		return E_FAIL;

	left = 0;
	right = width;
	top = 0;
	bottom = height;

	return S_OK;
}

HRESULT CMainWindow::GetWindowHandle(WindowHandle &winHandle)
{
	winHandle = _app->window;
	return S_OK;
}

HRESULT CMainWindow::GetWindowDrawContext(WindowDrawContext &dcHandle)
{
	dcHandle = _app->window;
	return S_OK;
}

HRESULT CMainWindow::BeginMainLoop()
{
	_BeginMainLoop();
	return S_OK;
}

HRESULT CMainWindow::KillWindow()
{
	_msgProcDel->Invoke(TGE::TWindowMessage(WMT_DESTROY));

	return S_OK;
}

HRESULT CMainWindow::Free()
{
	delete this;
	return S_OK;
}

HRESULT CMainWindow::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_MAIN_WINDOW;
	return S_OK;
}