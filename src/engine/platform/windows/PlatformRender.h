#ifndef _PLATFORMRENDER_H_
#define _PLATFORMRENDER_H_

#include "..\Common.h"

class CPlatformRender : public IPlatformRender, private CUncopyable
{
	CCore				*_pCore;
	HDC					_hDC;
	HGLRC				_hGLRC;

public:
	CPlatformRender(CCore *pCore);
	~CPlatformRender();

	HRESULT Initialize(const TWindowParams &winParams) override final;
	HRESULT Free() override final;
	HRESULT MakeCurrent() override final;
	HRESULT IsContextValid(bool &flag) override final;
	HRESULT IsContextChanged(bool &flag) override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};

#endif //_PLATFORMRENDER_H_