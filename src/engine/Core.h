#ifndef _CORE_H_
#define _CORE_H_

#include "Common.h"

class CInput;

class CCore : public ICore, private CUncopyable
{
	TProcDelegate		_delProcess,
						_delRender,
						_delInit,
						_delFree,
						_delMLoop;
	TMsgProcDelegate	_delMsgProc;
	std::fstream		_logFile;

	IMainWindow			*_pMainWindow;
	CInput				*_pInput;

	IPlatformRender		*_pPlatformRender;

	bool				_doExit;

	uint				_processInterval;
	uint64				_oldTime;

	CCore();
	~CCore();

	void _MLoop();
	void _MsgProc(const TWindowMessage& msg);

	static void CALLBACK _s_MLoop(void *pParam);
	static void CALLBACK _s_MsgProc(const TWindowMessage& msg, void *pParam);

	friend bool TGE::GetEngine(ICore *& pICore);
	friend void TGE::FreeEngine();

public:

	TMsgProcDelegate* pDMessageProc() { return &_delMsgProc; }
	IMainWindow* GetMainWindow() const;

	HRESULT CALLBACK InitializeEngine(const char *appName, const TWindowParams &winParams, E_ENGINE_INIT_FLAGS initFlags) override final;
	HRESULT CALLBACK QuitEngine() override final;
	HRESULT CALLBACK AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) override final;
	HRESULT CALLBACK RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) override final;
	HRESULT CALLBACK AddToLog(const char *txt, bool isError) override final;

	HRESULT CALLBACK GetEngineSubsystem(E_ENGINE_SUB_SYSTEM engSubsystemType, IEngineSubsystem *&pEngSubsystem) override final;
};

#endif //_CORE_H_