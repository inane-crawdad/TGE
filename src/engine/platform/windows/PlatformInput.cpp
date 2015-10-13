#include "PlatformInput.h"

CPlatformInput::CPlatformInput(CCore *pCore):
_pCore(pCore)
{
	_pCore->GetMainWindow()->GetWindowHandle(_hWnd);

	_pCore->AddToLog("Platform input subsystem was initialized!", false);
}
CPlatformInput::~CPlatformInput()
{
	_pCore->AddToLog("Platform input subsystem was finalized!", false);
}

HRESULT CPlatformInput::GetCursorPos(int &x, int &y)
{
	POINT curPos;
	::GetCursorPos(&curPos);
	::ScreenToClient(_hWnd, &curPos);
	x = curPos.x;
	y = curPos.y;
	return S_OK;
}

HRESULT CPlatformInput::SetCursorPos(int x, int y)
{
	POINT pt = { x, y };
	if (!::ClientToScreen(_hWnd, &pt))
		return E_FAIL;

	if (::SetCursorPos(pt.x, pt.y))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT CPlatformInput::ShowCursor(bool show)
{
	::ShowCursor(show);
	return S_OK;
}

HRESULT CPlatformInput::ClipCursor(TGE::int32 left, TGE::int32 right, TGE::int32 top, TGE::int32 bottom)
{
	RECT rect = { left, top, right, bottom };
	if (::ClipCursor(&rect))
		return S_OK;
	else
		return E_FAIL;
}

HRESULT CPlatformInput::GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst)
{
	pst = PSST_INPUT;
	return S_OK;
}

HRESULT CPlatformInput::Free()
{
	delete this;
	return S_OK;
}