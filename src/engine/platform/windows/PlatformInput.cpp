#include "PlatformInput.h"

CPlatformInput::CPlatformInput(CCore *pCore):
_pCore(pCore)
{
	_pCore->GetMainWindow()->GetWindowHandle(_hWnd);

	BYTE CursorMaskAND[] = { 0xFF };
	BYTE CursorMaskXOR[] = { 0x00 };
	_blankCursor = CreateCursor(::GetModuleHandle(NULL), 0, 0, 1, 1, CursorMaskAND, CursorMaskXOR);

	_pCore->AddToLog("Platform input subsystem was initialized!", false);
}
CPlatformInput::~CPlatformInput()
{
	if (_blankCursor)
		if (!::DestroyCursor(_blankCursor))
			_pCore->AddToLog("Can't delete cursor", false);

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

HRESULT CPlatformInput::GetCenter(int &x, int &y)
{
	RECT rc;
	_pCore->GetMainWindow()->GetClientRect(rc.left, rc.right, rc.top, rc.bottom);
	POINT center = { rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2 };
	::ScreenToClient(_hWnd, &center);
	x = center.x;
	y = center.y;

	return S_OK;
}

HRESULT CPlatformInput::ShowCursor(bool show)
{
	if (!show)
		::SetClassLong(_hWnd, GCL_HCURSOR, (LONG)_blankCursor);
	else
		::SetClassLong(_hWnd, GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
	
	return S_OK;
}

HRESULT CPlatformInput::ClipCursor(TGE::int32 left, TGE::int32 right, TGE::int32 top, TGE::int32 bottom)
{
	if (left == 0 && right == 0 && top == 0 && bottom == 0)
	{
		::ClipCursor(NULL);
		return S_OK;
	}

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