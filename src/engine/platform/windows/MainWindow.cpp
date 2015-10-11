#include "MainWindow.h"

CMainWindow::CMainWindow(CCore *pCore) :
	_pCore(pCore),
	_hInst(GetModuleHandle(NULL)),
	_hWnd(NULL),
	_hDC(NULL),
	_hGLRC(NULL),
	_msaaSamples(4u)
{}

CMainWindow::~CMainWindow()
{
	if (_hInst != NULL && !UnregisterClass("TGE Window Class", _hInst))
	{
		_hInst = NULL;
		_pCore->AddToLog("Can't unregister window class", false);
	}
	else
		_pCore->AddToLog("Window closed properly", false);
}

LRESULT CALLBACK CMainWindow::_s_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// get prior saved pointer to this object
	CMainWindow *this_ptr = (CMainWindow *)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (this_ptr)
	{
		if (msg == WM_DESTROY)
			PostQuitMessage(0);

		this_ptr->_msgProcDel->Invoke(WinAPIMsgToEngMsg(msg, wParam, lParam));

		if (msg == WM_CLOSE)
			return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CMainWindow::_BeginMainLoop()
{
	MSG msg = { 0 };
	bool isLooping = true;

	_pCore->AddToLog("*** Begin main loop ***", false);
	while (isLooping)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				isLooping = false;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			_mLoopDel->Invoke();
			SwapBuffers(_hDC);
		}

	}

	_pCore->AddToLog("*** Exit main loop ***", false);

	return (int)msg.wParam;
}

HRESULT CMainWindow::InitWindow(TProcDelegate *mLoopDelegate, TMsgProcDelegate *msgProcDelegate)
{
	_mLoopDel = mLoopDelegate;
	_msgProcDel = msgProcDelegate;
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = TEXT("TGE Window Class");
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = _hInst;
	wc.lpfnWndProc = (WNDPROC)CMainWindow::_s_WndProc;
	wc.hbrBackground = (HBRUSH)(0);
	wc.lpszMenuName = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (RegisterClassEx(&wc) == FALSE)
	{
		_pCore->AddToLog("Can't register window class", false);
		return E_FAIL;
	}

	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, TEXT("TGEApp"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, _hInst, NULL);

	if (!_hWnd)
	{
		_pCore->AddToLog("Can't create window", false);
		return E_FAIL;
	}

	if (!(_hDC = GetDC(_hWnd)))
	{
		_pCore->AddToLog("Can't get draw context", false);
		return E_FAIL;
	}

	// save pointer to this object in user data
	SetWindowLongPtr(_hWnd, GWL_USERDATA, (LONG_PTR)this);

	return S_OK;
}

HRESULT CMainWindow::ConfigureWindow(const TGE::TWindowParams &winParams)
{
	if (!_hWnd)
	{
		_pCore->AddToLog("Window's handle does not exist", false);
		return E_ABORT;
	}
	
	_pCore->AddToLog("Configuring window", false);

	DWORD style = WS_VISIBLE;
	DWORD style_ex = WS_EX_APPWINDOW;

	if (winParams.fullScreen)
		style |= WS_POPUP;
	else
		style |= WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;

	if (SetWindowLong(_hWnd, GWL_EXSTYLE, style_ex) == 0)
	{
		_pCore->AddToLog("Can't set window ex style", false);
		return E_ABORT;
	}

	if (SetWindowLong(_hWnd, GWL_STYLE, style) == 0)
	{
		_pCore->AddToLog("Can't set window style", false);
		return E_ABORT;
	}

	RECT rc = { 0, 0, winParams.width, winParams.height };
	AdjustWindowRectEx(&rc, style, FALSE, style_ex);

	int x_pos = 0, y_pos = 0;

	if (!winParams.fullScreen)
	{
		uint resX, resY;
		GetDisplaySize(resX, resY);

		x_pos = (resX - (rc.right - rc.left)) / 2;
		y_pos = (resY - (rc.bottom - rc.top)) / 2;

		if (x_pos < 0) x_pos = 0;
		if (y_pos < 0) y_pos = 0;
	}

	::SetWindowPos(_hWnd, HWND_TOP, x_pos, y_pos, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);
	::SetCursorPos(x_pos + (rc.right - rc.left) / 2, y_pos + (rc.bottom - rc.top) / 2);

	SetForegroundWindow(_hWnd);

	return S_OK;
}

HRESULT CMainWindow::SetCaption(const char *pCaption)
{
	SetWindowText(_hWnd, pCaption);
	return S_OK;
}

HRESULT CMainWindow::GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom)
{
	if (!_hWnd)
		return E_FAIL;

	RECT rect;

	::GetClientRect(_hWnd, &rect);

	POINT lt, rb;
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	::ClientToScreen(_hWnd, &lt);
	::ClientToScreen(_hWnd, &rb);

	left = lt.x;
	right = rb.x;
	top = lt.y;
	bottom = rb.y;

	return S_OK;
}

HRESULT CMainWindow::GetWindowHandle(WindowHandle &winHandle)
{
	winHandle = _hWnd;
	return S_OK;
}

HRESULT CMainWindow::GetWindowDrawContext(TGE::WindowDrawContext &dcHandle)
{
	dcHandle = _hDC;
	return S_OK;
}

HRESULT CMainWindow::BeginMainLoop()
{
	return _BeginMainLoop() != -1 ? S_OK : E_FAIL;
}

HRESULT CMainWindow::KillWindow()
{
	if (_hDC != NULL && !ReleaseDC(_hWnd, _hDC))
		_pCore->AddToLog("Can't release device context", false);

	if (_hWnd != NULL && !DestroyWindow(_hWnd))
	{
		_pCore->AddToLog("Can't destroy window", false);
		return E_ABORT;
	}
		
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