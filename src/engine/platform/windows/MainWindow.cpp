#include "MainWindow.h"

CMainWindow::CMainWindow(ICore *pCore) :
	_pCore(pCore),
	_msaaSamples(4u),
	_hInst(GetModuleHandle(NULL)),
	_hDC(NULL),
	_hWnd(NULL),
	_hGLRC(NULL)
{}

CMainWindow::~CMainWindow()
{
	if (_hGLRC)
	{
		if(!wglMakeCurrent(NULL, NULL))
			_pCore->AddToLog("Can't release device context and render context", false);
		
		if (!wglDeleteContext(_hGLRC))
			_pCore->AddToLog("Can't properly delete render context", false);
	}


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
			_procDel->Invoke();
			SwapBuffers(_hDC);
		}

	}

	_pCore->AddToLog("*** Exit main loop ***", false);

	return (int)msg.wParam;
}

HRESULT CMainWindow::InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate)
{
	_procDel = procDelegate;
	_msgProcDel = msgProcDelegate;
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = "TGE Window Class";
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

	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, "TGEApp", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, _hInst, NULL);

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

	uint msaa_samples = _msaaSamples;

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixel_format = NULL;

	if (msaa_samples > 1)
	{
		HWND temp_wnd;
		HDC temp_dc;
		HGLRC temp_rc;
		int temp_pixel_format;

		if (
			!(temp_wnd = CreateWindowEx(0, "STATIC", NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL)) ||
			!(temp_dc = GetDC(temp_wnd)) ||
			!(temp_pixel_format = ChoosePixelFormat(temp_dc, &pfd)) ||
			!SetPixelFormat(temp_dc, temp_pixel_format, &pfd) ||
			!(temp_rc = wglCreateContext(temp_dc)) ||
			!wglMakeCurrent(temp_dc, temp_rc)
			)
		{
			msaa_samples = 1;
			_pCore->AddToLog("Error(s) while performing OpenGL MSAA preinit routine", false);
		}
		else
		{
			if (std::string((char *)glGetString(GL_EXTENSIONS)).find("ARB_multisample") != std::string::npos)
			{
				int attribs[] = {
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB, 32,
					WGL_ALPHA_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 0,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_SAMPLES_ARB, msaa_samples,
					0, 0
				};

				float float_attribs[] = { 0.0 };
				unsigned int formats_count;
				int tmp_pixel_format;

				wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
				BOOL valid = wglChoosePixelFormatARB(temp_dc, attribs, float_attribs, 1, &tmp_pixel_format, &formats_count);
				wglChoosePixelFormatARB = NULL;

				if (valid && formats_count >= 1)
					pixel_format = tmp_pixel_format;
				else
				{
					_pCore->AddToLog("Can't find pixel format for required number of samples", false);
					msaa_samples = 1;
				}
			}
			else
				msaa_samples = 1;
		}
		if (
			!wglMakeCurrent(NULL, NULL) ||
			(temp_rc != NULL && !wglDeleteContext(temp_rc)) ||
			(temp_dc != NULL && !ReleaseDC(temp_wnd, temp_dc)) ||
			(temp_wnd != NULL && !DestroyWindow(temp_wnd))
			)
			_pCore->AddToLog("Error(s) while cleaning resources for OpenGL MSAA preinit routine", false);
	}

	if (pixel_format == NULL && (pixel_format = ChoosePixelFormat(_hDC, &pfd)))
	{
		_pCore->AddToLog("Can't choose pixel format", false);
		return E_ABORT;
	}

	if (!SetPixelFormat(_hDC, pixel_format, &pfd))
	{
		_pCore->AddToLog("Can't set pixel format", false);
		return E_ABORT;
	}

	if (!(_hGLRC = wglCreateContext(_hDC)))
	{
		_pCore->AddToLog("Can't create render context", false);
		return E_ABORT;
	}

	if (!wglMakeCurrent(_hDC, _hGLRC))
	{
		_pCore->AddToLog("Can't make current render context", false);
		return E_ABORT;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		_pCore->AddToLog("Can't make current render context", false);
		return E_ABORT;
	}

	if (WGLEW_EXT_swap_control)
		wglSwapIntervalEXT(1);

	return S_OK;
}

HRESULT CMainWindow::ConfigureWindow(uint resX, uint resY, bool fullScreen)
{
	if (!_hWnd)
	{
		_pCore->AddToLog("Window's handle does not exist", false);
		return E_ABORT;
	}
	
	_pCore->AddToLog("Configuring window", false);

	DWORD style = WS_VISIBLE;
	DWORD style_ex = WS_EX_APPWINDOW;

	if (fullScreen)
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

	RECT rc = { 0, 0, resX, resY };
	AdjustWindowRectEx(&rc, style, FALSE, style_ex);

	SetWindowPos(_hWnd, HWND_TOP, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED);

	SetForegroundWindow(_hWnd);

	return S_OK;
}

HRESULT CMainWindow::SetCaption(const char *pCaption)
{
	SetWindowText(_hWnd, pCaption);
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

//void CMainWindow::_OnResizeWindow(void)
//{
//	RECT rect;
//	GetClientRect(_hWnd, &rect);
//	glViewport(0, 0, rect.right, rect.bottom);
//}
