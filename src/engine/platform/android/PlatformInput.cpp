#include "PlatformInput.h"

#include "GestureDetector.h"

CPlatformInput::CPlatformInput(CCore *pCore) :
_pCore(pCore)
{
	_pTapDetector		= new CTapDetector();
	_pDoubleTapDetector = new CDoubleTapDetector();
	_pDragDetector		= new CDragDetector();
	_pPinchDetector		= new CPinchDetector();

	_pCore->AddToLog("Platform input subsystem was initialized!", false);
}

CPlatformInput::~CPlatformInput()
{
	delete _pTapDetector;
	delete _pDoubleTapDetector;
	delete _pDragDetector;
	delete _pPinchDetector;

	_pCore->AddToLog("Platform input subsystem was finalized!", false);
}

HRESULT CPlatformInput::Configure(const TTouchParams& touchParams)
{
	_pTapDetector->Configure(touchParams);
	_pDoubleTapDetector->Configure(touchParams);
	return S_OK;
}
HRESULT CPlatformInput::GetGestureDetector(E_GESTURE_DETECTOR_TYPE type, IGestureDetector *&pGestureDetector)
{
	switch (type)
	{
	case GDT_TAP:
		pGestureDetector = _pTapDetector;
		break;
	case GDT_DOUBLE_TAP:
		pGestureDetector = _pDoubleTapDetector;
		break;
	case GDT_DRAG:
		pGestureDetector = _pDragDetector;
		break;
	case GDT_PINCH:
		pGestureDetector = _pPinchDetector;
		break;
	default:
		return E_INVALIDARG;
		break;
	}

	return S_OK;
}

HRESULT CPlatformInput::Free()
{
	delete this;
	return S_OK;
}

HRESULT CPlatformInput::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_INPUT;
	return S_OK;
}