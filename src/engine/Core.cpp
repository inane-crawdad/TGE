#include "Core.h"
#include "platform\windows\MainWindow.h"

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
_doExit(false)
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
	if (!(initFlags & E_ENGINE_INIT_FLAGS::EIF_NO_LOG))
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
		if (FAILED(_pMainWindow->ConfigureWindow(resX, resY, (E_ENGINE_INIT_FLAGS::EIF_FULLSCREEN & initFlags))))
			return E_ABORT;

		AddToLog("Engine initializing is done!", false);

		if (!_delInit.IsEmpty())
		{
			AddToLog("Start user's init procedure", false);
			_delInit.Invoke();
			AddToLog("Done!", false);
		}

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
	case TGE::FT_PROCESS:
		_delProcess.Add(func, pParam);
		break;
	case TGE::FT_RENDER:
		_delRender.Add(func, pParam);
		break;
	case TGE::FT_INIT:
		_delInit.Add(func, pParam);
		break;
	case TGE::FT_FREE:
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
	case TGE::FT_PROCESS:
		_delProcess.Remove(func, pParam);
		break;
	case TGE::FT_RENDER:
		_delRender.Remove(func, pParam);
		break;
	case TGE::FT_INIT:
		_delInit.Remove(func, pParam);
		break;
	case TGE::FT_FREE:
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

void CCore::_s_MsgProc(const TGE::TWindowMessage& msg, void *pParam)
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
}

void CCore::_MsgProc(const TGE::TWindowMessage& msg)
{
	switch (msg.messageType)
	{
	case(TGE::E_WINDOW_MESSAGE_TYPE::WMT_CLOSE) :
		_doExit = true;
		break;
	case(TGE::E_WINDOW_MESSAGE_TYPE::WMT_DESTROY) :

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

// Testing


void CCore::TestDelegates()
{
	_delInit.Invoke();
	_delFree.Invoke();
	_delProcess.Invoke();
	_delRender.Invoke();

	_delMsgProc.Invoke(TGE::TWindowMessage());
	//_delMsgProc.Remove(MsgProcFunc, this);
}
//