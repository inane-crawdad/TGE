#ifndef _PLATFORMINPUT_H_
#define _PLATFORMINPUT_H_

#include "..\..\Common.h"

class CTapDetector;
class CDoubleTapDetector;
class CDragDetector;
class CPinchDetector;

class CPlatformInput : public IPlatformInput, private CUncopyable
{
	CCore		*_pCore;

	CTapDetector		*_pTapDetector;
	CDoubleTapDetector	*_pDoubleTapDetector;
	CDragDetector		*_pDragDetector;
	CPinchDetector		*_pPinchDetector;

public:
	CPlatformInput(CCore *pCore);
	~CPlatformInput();

	HRESULT Configure(const TTouchParams& touchParams) override final;
	HRESULT GetGestureDetector(E_GESTURE_DETECTOR_TYPE type, IGestureDetector *&pGestureDetector) override final;
	HRESULT Free() override final;

	HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) override final;
};


#endif