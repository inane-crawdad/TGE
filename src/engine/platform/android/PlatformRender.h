#ifndef _PLATFORMRENDER_H_
#define _PLATFORMRENDER_H_

#include "../../Common.h"

class CPlatformRender : public IPlatformRender, private CUncopyable
{
	CCore				*_pCore;

	ANativeWindow		*_pWindow;
	EGLDisplay			_display;
	EGLSurface			_surface;
	EGLContext			_context;
	EGLConfig			_config;

	EGLint				_currentWidth;
	EGLint				_currentHeight;

	bool				_isInitialized;
	bool				_isContextValid;
	bool				_isContextChanged;

	uint8				_msaaCount;
	uint8				_depthBits;

	void _FindMaxMsaaAndDepth();
	void _InitSurface();
	void _InitContext();
	void _Resume();
	void _Suspend();
	void _Terminate();

	void _MsgProc(const TWindowMessage& msg);
	static void CALLBACK _s_MsgProc(const TWindowMessage& msg, void *pParam);

public:
	CPlatformRender(CCore *pCore);
	~CPlatformRender();

	void SwapBuffers();
	void GetWindowSize(EGLint &width, EGLint &height);
	bool IsSizeChanged();

	HRESULT Initialize(const TWindowParams &winParams) override final;
	HRESULT Free() override final;
	HRESULT MakeCurrent() override final;
	HRESULT IsContextValid(bool &flag) override final;
	HRESULT IsContextChanged(bool &flag) override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};

#endif //_PLATFORMRENDER_H_