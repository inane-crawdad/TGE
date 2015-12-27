#include "PlatformRender.h"

CPlatformRender::CPlatformRender(CCore *pCore) :
_pCore(pCore),
_pWindow(nullptr),
_display(EGL_NO_DISPLAY),
_surface(EGL_NO_SURFACE),
_context(EGL_NO_CONTEXT),
_currentWidth(0),
_currentHeight(0),
_config(nullptr),
_isInitialized(false),
_isContextValid(false),
_isContextChanged(false),
_msaaCount(0),
_depthBits(0)
{}

CPlatformRender::~CPlatformRender()
{
	_Terminate();
	_pCore->AddToLog("Platform render subsystem was finalized!", false);
}

HRESULT CPlatformRender::Initialize(const TWindowParams &winParams)
{
	if (winParams.pParam == nullptr)
	{
		_pCore->AddToLog("There is no android_app handle", false);
		return E_INVALIDARG;
	}

	_msaaCount = winParams.msaa_count;
	_pCore->pDMessageProc()->Add(_s_MsgProc, this);
	
	_pCore->AddToLog("Platform render subsystem was initialized!", false);

	return S_OK;
}

HRESULT CPlatformRender::Free()
{
	delete this;
	return S_OK;
}

HRESULT CPlatformRender::MakeCurrent()
{
	if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
		return E_FAIL;

	return S_OK;
}

HRESULT CPlatformRender::IsContextValid(bool &flag)
{
	flag = _isContextValid;
	return S_OK;
}

HRESULT CPlatformRender::IsContextChanged(bool &flag)
{
	flag = _isContextChanged;
	_isContextChanged = false;
	return S_OK;
}

HRESULT CPlatformRender::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_RENDER;
	return S_OK;
}

void CPlatformRender::_FindMaxMsaaAndDepth()
{
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};

	EGLint numConfigs;
	if (!eglGetConfigs(_display, 0, 0, &numConfigs))
	{
		_pCore->AddToLog("Can't get frame buffer configs!", false);
		_msaaCount = 0;
		return;
	}
	
	EGLint num = 0, maxMsaa = 0, maxDepth = 0;
	EGLConfig *configs = new EGLConfig[numConfigs];
	if (eglChooseConfig(_display, attribs, configs, numConfigs, &num))
	{
		for (int i = 0; i < num; ++i)
		{
			EGLint msaa, depth;
			eglGetConfigAttrib(_display, configs[i], EGL_DEPTH_SIZE, &depth);
			eglGetConfigAttrib(_display, configs[i], EGL_SAMPLES, &msaa);
			if (msaa >= maxMsaa && depth >= maxDepth)
			{
				maxMsaa = msaa;
				maxDepth = depth;
			}
		}
	}
	delete[] configs;
	if (num == 0)
		_pCore->AddToLog("Can't find any frame buffer configs with 24 bits for color!", false);
	if (maxMsaa < _msaaCount)
		_msaaCount = maxMsaa;
	_depthBits = maxDepth;
	
}

void CPlatformRender::_InitSurface()
{
	_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (_display == EGL_NO_DISPLAY)
	{
		_pCore->AddToLog("Can't get default display!", false);
		return;
	}
	if (!eglInitialize(_display, 0, 0))
	{
		_pCore->AddToLog("Can't initialize EGL!", false);
		return;
	}

	if (!_isInitialized)
		_FindMaxMsaaAndDepth();

	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, (EGLint)_depthBits,
		EGL_SAMPLES, (EGLint)_msaaCount,
		EGL_NONE
	};

	EGLint format, numConfigs = 0;

	eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);
	if (numConfigs == 0)
	{
		_pCore->AddToLog("Can't find suitable frame buffer config! Try to reduce attribs", false);
		const EGLint reduced_attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
		};
		eglChooseConfig(_display, reduced_attribs, &_config, 1, &numConfigs);
		if (numConfigs == 0)
		{
			_pCore->AddToLog("Can't find suitable frame buffer config with reduced attribs!", false);
			return;
		}
	}
	if (!eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format))
	{
		_pCore->AddToLog("Can't get native visual id!", false);
		return;
	}
	ANativeWindow_setBuffersGeometry(_pWindow, 0, 0, format);

	_surface = eglCreateWindowSurface(_display, _config, _pWindow, NULL);
	if (_surface == EGL_NO_SURFACE)
	{
		_pCore->AddToLog("Can't create surface!", false);
		return;
	}
}

void CPlatformRender::_InitContext()
{
	const EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE };
	_context = eglCreateContext(_display, _config, EGL_NO_CONTEXT, context_attribs);
	if (_context == EGL_NO_CONTEXT)
	{
		_isContextValid = false;
		_pCore->AddToLog("Can't create context!", false);
		return;
	}

	if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
	{
		_isContextValid = false;
		_pCore->AddToLog("Unable make context current!", false);
		return;
	}
	_isContextValid = true;
}

void CPlatformRender::_Resume()
{
	_pCore->GetMainWindow()->GetWindowHandle(_pWindow);
	if (!_isInitialized)
	{
		_InitSurface();
		_InitContext();
		_isInitialized = true;
		GetWindowSize(_currentWidth, _currentHeight);
		return;
	}

	if (_surface == EGL_NO_SURFACE)
		_surface = eglCreateWindowSurface(_display, _config, _pWindow, NULL);

	if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_TRUE)
		return;

	EGLint err = eglGetError();

	if (err == EGL_CONTEXT_LOST)
		_InitContext();
	else
	{
		_Terminate();
		_InitSurface();
		_InitContext();
	}
	_isContextChanged = true;
	GetWindowSize(_currentWidth, _currentHeight);
}

void CPlatformRender::_Suspend()
{
	if (_surface != EGL_NO_SURFACE)
	{
		eglDestroySurface(_display, _surface);
		_surface = EGL_NO_SURFACE;
	}
}

void CPlatformRender::_Terminate()
{
	if (_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (_context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(_display, _context);
		}

		if (_surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(_display, _surface);
		}
		eglTerminate(_display);
	}

	_display = EGL_NO_DISPLAY;
	_context = EGL_NO_CONTEXT;
	_surface = EGL_NO_SURFACE;
	_isContextValid = false;
}

void CPlatformRender::_s_MsgProc(const TWindowMessage& msg, void *pParam)
{
	((CPlatformRender*)pParam)->_MsgProc(msg);
}

void CPlatformRender::_MsgProc(const TWindowMessage& msg)
{
	switch (msg.messageType)
	{
	case WMT_CREATE :
		_Resume();
		break;
	case WMT_RELEASED :
		_Suspend();
		break;
	default:
		break;
	}
}

void CPlatformRender::SwapBuffers()
{
	if (!eglSwapBuffers(_display, _surface))
	{
		EGLint err = eglGetError();
		if (err == EGL_BAD_SURFACE)
			_InitSurface();
		else if (err == EGL_CONTEXT_LOST)
		{
			_InitContext();
			_isContextChanged = true;
		}
		else
		{
			_Terminate();
			_InitSurface();
			_InitContext();
			_isContextChanged = true;
		}
	}
}

void CPlatformRender::GetWindowSize(EGLint &width, EGLint &height)
{
	if (_isInitialized)
	{
		eglQuerySurface(_display, _surface, EGL_WIDTH, &width);
		eglQuerySurface(_display, _surface, EGL_HEIGHT, &height);
	}
}

bool CPlatformRender::IsSizeChanged()
{
	EGLint oldWidth = _currentWidth, oldHeight = _currentHeight;
	GetWindowSize(_currentWidth, _currentHeight);
	if (oldWidth != _currentWidth || oldHeight != _currentHeight)
		return true;
	return false;
}