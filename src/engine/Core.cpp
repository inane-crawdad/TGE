#include "Core.h"

#ifdef PLATFORM_WINDOWS
#include "platform\windows\MainWindow.h"
#include "platform\windows\Input.h"
#endif

CCore *pCore = nullptr;

bool TGE::GetEngine(ICore *& pICore)
{
	if (pCore == nullptr)
	{
		pCore = new CCore();
		pICore = (ICore *&)pCore;
	}
	else
		pICore = (ICore *&)pCore;

	return true;
}

void TGE::FreeEngine()
{
	if (pCore != nullptr)
	{
		delete pCore;
		pCore = nullptr;
	}
}

CCore::CCore():
_doExit(false),
_processInterval(16),
_pInput(nullptr)
{
	_delMLoop.Add(_s_MLoop, this);
	_delMsgProc.Add(_s_MsgProc, this);

	_pMainWindow = new CMainWindow(this);
}

CCore::~CCore()
{
	_pMainWindow->Free();

	if (_logFile.is_open())
	{
		_logFile << "Log closed" << std::endl;
		_logFile.close();
	}
}

HRESULT CALLBACK CCore::InitializeEngine(uint resX, uint resY, const char *appName, E_ENGINE_INIT_FLAGS initFlags)
{
	if (!(initFlags & EIF_NO_LOG))
	{
		_logFile.setf(std::ios_base::adjustfield);
		_logFile.open("log.txt", std::ios::out | std::ios::trunc);

		TSysTime t;
		GetLocalSysTime(t);

		_logFile << "TGE log file" << std::endl;
		_logFile << "Log started at " << t.hour << "." << t.minute << "." << t.second << std::endl;
	}

	if (SUCCEEDED(_pMainWindow->InitWindow(&_delMLoop, &_delMsgProc)))
	{
		_pMainWindow->SetCaption(appName);

		if ((EIF_FULLSCREEN & initFlags) && (EIF_NATIVE_RESOLUTION & initFlags))
			GetDisplaySize(resX, resY);

		if (FAILED(_pMainWindow->ConfigureWindow(resX, resY, (EIF_FULLSCREEN & initFlags))))
			return E_ABORT;

		_pInput = new CInput(this);

		AddToLog("Engine initializing is done!", false);

		if (!_delInit.IsEmpty())
		{
			AddToLog("Start user's init procedure", false);
			_delInit.Invoke();
			AddToLog("Done!", false);
		}

		_oldTime = GetPerfTimer() / 1000;
		_pMainWindow->BeginMainLoop();
	}
	else
		return E_ABORT;

	return S_OK;
}
HRESULT CALLBACK CCore::QuitEngine()
{
	_doExit = true;
	return S_OK;
}
HRESULT CALLBACK CCore::AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam)
{
	switch (funcType)
	{
	case FT_PROCESS:
		_delProcess.Add(func, pParam);
		break;
	case FT_RENDER:
		_delRender.Add(func, pParam);
		break;
	case FT_INIT:
		_delInit.Add(func, pParam);
		break;
	case FT_FREE:
		_delFree.Add(func, pParam);
		break;
	default:
		return E_INVALIDARG;
	}

	return S_OK;
}
HRESULT CALLBACK CCore::RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam)
{
	switch (funcType)
	{
	case FT_PROCESS:
		_delProcess.Remove(func, pParam);
		break;
	case FT_RENDER:
		_delRender.Remove(func, pParam);
		break;
	case FT_INIT:
		_delInit.Remove(func, pParam);
		break;
	case FT_FREE:
		_delFree.Remove(func, pParam);
		break;
	default:
		return E_INVALIDARG;
	}

	return S_OK;
}
HRESULT CALLBACK CCore::AddToLog(const char *txt, bool isError)
{
	if (_logFile.is_open())
	{
		TSysTime t;
		GetLocalSysTime(t);

		_logFile << t.hour << ":";
		_logFile << t.minute << ":";
		_logFile << t.second << ":";
		_logFile << t.milliSecond << " - ";
		_logFile << txt << std::endl;

		if (isError)
		{
			_logFile.flush();
			Terminate();
		}
	}
	return S_OK;
}

void CCore::_s_MLoop(void *pParam)
{
	((CCore*)pParam)->_MLoop();
}

void CCore::_s_MsgProc(const TWindowMessage& msg, void *pParam)
{
	((CCore*)pParam)->_MsgProc(msg);
}

void CCore::_MLoop()
{
	if (_doExit)
	{
		_pMainWindow->KillWindow();
		return;
	}

	auto time = GetPerfTimer() / 1000;
	auto timeDelta = time - _oldTime;

	bool flag = false;
	uint cycles_count = (uint)(timeDelta / _processInterval);
	for (uint i = 0; i < cycles_count; i++)
	{
		if (!_delProcess.IsEmpty())
			_delProcess.Invoke();
		flag = true;
	}

	if(flag)
		_oldTime = time - timeDelta % _processInterval;

	_delRender.Invoke();
}

void CCore::_MsgProc(const TWindowMessage& msg)
{
	switch (msg.messageType)
	{
	case(WMT_CLOSE) :
		_doExit = true;
		break;
	case(WMT_REDRAW) :
		_delMLoop.Invoke();
		break;
	case(WMT_DESTROY) :

		AddToLog("Finalizing engine ...", false);

		if (!_delFree.IsEmpty())
		{
			AddToLog("Start user's free procedure", false);
			_delFree.Invoke();
			AddToLog("Done!", false);
		}
		
		break;
	default:
		break;
	}
}

HRESULT CCore::GetInput(IInput *&pInput)
{
	if (_pInput == nullptr)
		return E_ABORT;

	pInput = _pInput;
	return S_OK;
}

IMainWindow* CCore::GetMainWindow() const
{
	return _pMainWindow;
}
