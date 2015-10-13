#include "PlatformSubsystemManager.h"

#ifdef PLATFORM_WINDOWS
#include "windows\MainWindow.h"
#include "windows\PlatformRender.h"
#include "windows\PlatformInput.h"
#endif

HRESULT CPlatformSubsystemManager::GetPlatformSubsystem(TGE::ICore *pCore, E_PLATFORM_SUB_SYSTEM_TYPE type, IPlatformSubsystem *&pPSS)
{
	static CMainWindow *s_pMainWindow = nullptr;
	static CPlatformRender *s_pPlatformRender = nullptr;
	static CPlatformInput *s_pPlatformInput = nullptr;

	switch (type)
	{
	case PSST_MAIN_WINDOW:
		if (s_pMainWindow == nullptr)
		{
			s_pMainWindow = new CMainWindow(static_cast<CCore*>(pCore));
			pPSS = s_pMainWindow;
		}
		else
			pPSS = s_pMainWindow;
		break;
	case PSST_INPUT:
		if (s_pPlatformInput == nullptr)
		{
			s_pPlatformInput = new CPlatformInput(static_cast<CCore*>(pCore));
			pPSS = s_pPlatformInput;
		}
		else
			pPSS = s_pPlatformInput;
		break;
	case PSST_RENDER:
		if (s_pPlatformRender == nullptr)
		{
			s_pPlatformRender = new CPlatformRender(static_cast<CCore*>(pCore));
			pPSS = s_pPlatformRender;
		}
		else
			pPSS = s_pPlatformRender;
		break;
	default:
		return E_INVALIDARG;
		break;
	}
	return S_OK;
}