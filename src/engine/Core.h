#ifndef _CORE_H_
#define _CORE_H_

#include "Common.h"

class CCore : public ICore
{
	TProcDelegate		_delProcess,
						_delRender,
						_delInit,
						_delFree,
						_delMLoop;
	TMsgProcDelegate	_delMsgProc;
	std::fstream		_logFile;

	IMainWindow			*_pMainWindow;

	bool				_doExit;

	CCore();
	CCore(const CCore&){}
	CCore& operator=(const CCore&){}
	~CCore();

	friend bool TGE::GetEngine(ICore *& pICore);
	friend void TGE::FreeEngine();

public:

	HRESULT CALLBACK InitializeEngine(uint resX, uint resY, const char *appName, E_ENGINE_INIT_FLAGS initFlags) override final;
	HRESULT CALLBACK QuitEngine() override final;
	HRESULT CALLBACK AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) override final;
	HRESULT CALLBACK RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) override final;
	HRESULT CALLBACK AddToLog(const char *txt, bool isError) override final;

	void TestDelegates() override final;
};

#endif //_CORE_H_