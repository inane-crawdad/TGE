#include "PlatformRender.h"

CPlatformRender::CPlatformRender(CCore *pCore):
_pCore(pCore),
_hDC(NULL),
_hGLRC(NULL)
{}

CPlatformRender::~CPlatformRender()
{
	if (_hGLRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
			_pCore->AddToLog("Can't release device context and render context", false);

		if (!wglDeleteContext(_hGLRC))
			_pCore->AddToLog("Can't properly delete render context", false);
	}
}

HRESULT CPlatformRender::Initialize(const TWindowParams &winParams)
{
	_pCore->GetMainWindow()->GetWindowDrawContext(_hDC);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion		= 1;
	pfd.dwFlags			= PFD_DOUBLEBUFFER |
						PFD_DRAW_TO_WINDOW |
						PFD_SUPPORT_OPENGL |
						PFD_GENERIC_ACCELERATED;
	pfd.iPixelType		= PFD_TYPE_RGBA;
	pfd.cColorBits		= 24;
	pfd.cRedBits		= 8;
	pfd.cGreenBits		= 8;
	pfd.cBlueBits		= 8;
	pfd.cDepthBits		= 32;

	int pixel_format = NULL;

	if (winParams.msaa_count != MSC_NONE)
	{
		HWND temp_wnd;
		HDC temp_dc;
		HGLRC temp_rc;
		int temp_pixel_format;

		if (
			!(temp_wnd = CreateWindowEx(0, TEXT("STATIC"), NULL, 0, 0, 0, 0, 0, 0, 0, 0, NULL)) ||
			!(temp_dc = GetDC(temp_wnd)) ||
			!(temp_pixel_format = ChoosePixelFormat(temp_dc, &pfd)) ||
			!SetPixelFormat(temp_dc, temp_pixel_format, &pfd) ||
			!(temp_rc = wglCreateContext(temp_dc)) ||
			!wglMakeCurrent(temp_dc, temp_rc)
			)
		{
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
					WGL_SAMPLES_ARB, (int)winParams.msaa_count * 2,
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
				}
			}
		}
		if (
			!wglMakeCurrent(NULL, NULL) ||
			(temp_rc != NULL && !wglDeleteContext(temp_rc)) ||
			(temp_dc != NULL && !ReleaseDC(temp_wnd, temp_dc)) ||
			(temp_wnd != NULL && !DestroyWindow(temp_wnd))
			)
			_pCore->AddToLog("Error(s) while cleaning resources for OpenGL MSAA preinit routine", false);
	}

	if (pixel_format == NULL && !(pixel_format = ChoosePixelFormat(_hDC, &pfd)))
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

	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	if (OpenGLVersion[0] < 3 || (OpenGLVersion[0] == 3 && OpenGLVersion[1] < 3))
	{
		_pCore->AddToLog("Need graphic card supporting OpenGL at least 3.3 version", false);
		return E_ABORT;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		_pCore->AddToLog("Can't make current render context", false);
		return E_ABORT;
	}

	if (winParams.vSync)
		if (WGLEW_EXT_swap_control)
			wglSwapIntervalEXT(1);
		else
			_pCore->AddToLog("VSync is not supported", false);

	return S_OK;
}

HRESULT CPlatformRender::Free()
{
	delete this;
	return S_OK;
}

HRESULT CPlatformRender::MakeCurrent()
{
	if (_hGLRC != NULL && _hDC != NULL)
	{
		if (wglGetCurrentContext() != _hGLRC)
		{
			wglMakeCurrent(_hDC, _hGLRC);
			return S_OK;
		}
		else
			return S_OK;
	}
	else
		return E_ABORT;
}

HRESULT CPlatformRender::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_RENDER;
	return S_OK;
}