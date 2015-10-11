#ifndef _TGE_H_
#define _TGE_H_

#if  defined(_WIN32) || defined(_WIN64)

#define PLATFORM_WINDOWS
#include <Windows.h>
#define STRUCT_ALIGN_1

#else
#error("Unknown platform")
#endif

#include "TGE_CommonTypes.h"

namespace TGE
{
	// main interface of engine
	class ITGE_Base{};

	enum E_ENGINE_SUB_SYSTEM
	{
		ESS_RENDER = 0,
		ESS_INPUT
	};

	class IEngineSubsystem : public ITGE_Base
	{
		virtual HRESULT CALLBACK GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType) = 0;
	};

	enum E_FUNC_TYPE
	{
		FT_PROCESS = 0,
		FT_RENDER,
		FT_INIT,
		FT_FREE
	};

	enum E_ENGINE_INIT_FLAGS
	{
		EIF_DEFAULT				= 0x00000000,
		EIF_NO_LOG				= 0x00000001
	};

	class IInput;

	class ICore : public ITGE_Base
	{
	public:
		virtual HRESULT CALLBACK InitializeEngine(const char *appName, const TWindowParams &winParams, E_ENGINE_INIT_FLAGS initFlags) = 0;
		virtual HRESULT CALLBACK QuitEngine() = 0;
		virtual HRESULT CALLBACK AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK AddToLog(const char *txt, bool isError) = 0;

		virtual HRESULT CALLBACK GetEngineSubsystem(E_ENGINE_SUB_SYSTEM engSubsystemType, IEngineSubsystem *&pEngSubsystem) = 0;
	};

#ifdef PLATFORM_WINDOWS

	enum E_INPUT_CONFIGURATION_FLAGS
	{
		ICF_DEFAULT				= 0x00000000,
		ICF_EXCLUSIVE			= 0x00000001,
		ICF_HIDE_CURSOR			= 0x00000002,
		ICF_ALWAYS_IN_CENTER	= 0x00000004
	};

	class IInput : public IEngineSubsystem
	{
	public:
		virtual HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flags = ICF_DEFAULT) = 0;
		virtual HRESULT CALLBACK GetKey(E_KEYBOARD_KEY_CODES eKey, bool &isPressed) = 0;
		virtual HRESULT CALLBACK GetMouseState(TMouseState& mouseState) = 0;
	};
#endif
	
#ifdef __cplusplus
	extern"C"
	{
#endif
		__declspec(dllexport) bool GetEngine(TGE::ICore *& pICore);
		__declspec(dllexport) void FreeEngine();
#ifdef __cplusplus
	}
#endif
}


#endif //_TGE_H_