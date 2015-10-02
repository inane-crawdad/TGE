#ifndef _TGE_H_
#define _TGE_H_

#if  defined(_WIN32) || defined(_WIN64)

#define PLATFORM_WINDOWS
#include <Windows.h>
#define STRUCT_ALIGN_1

#else
#error("Unknown platform")
#endif

#ifndef PLATFORM_WINDOWS
#define CALLBACK

#define S_OK			0L

#define S_FALSE			1L

#define E_FAIL			0x80004005L

#define E_ABORT			0x80004004L

#define E_INVALIDARG	0x80070057L

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif

namespace TGE
{
	//signed//
	typedef short int			int16;
	typedef long int			int32;
	typedef signed char			int8;

	//unsigned//
	typedef unsigned int		uint;
	typedef unsigned short int	uint16;
	typedef unsigned long int	uint32;
	typedef unsigned char		uint8;
	typedef uint8				uchar;


	// main interface of engine
	class ITGE_Base{};


	// types of messages
	enum E_WINDOW_MESSAGE_TYPE
	{
		WMT_UNKNOWN = 0,
		WMT_CLOSE,
		WMT_DESTROY,
		WMT_SIZE,
		WMT_KEY_DOWN,
		WMT_KEY_UP,
		WMT_MOUSE_MOVE,
		WMT_MOUSE_DOWN,
		WMT_MOUSE_UP,
		WMT_MOUSE_WHEEL
	};
#if defined(PLATFORM_WINDOWS) && defined(STRUCT_ALIGN_1)
#pragma pack(push,1)
#endif
	// abstraction above platform messages
	struct TWindowMessage
	{
		E_WINDOW_MESSAGE_TYPE	 messageType;
		uint32					 param1;
		uint32					 param2;
		void					*pParam3;

		TWindowMessage() :messageType(E_WINDOW_MESSAGE_TYPE::WMT_UNKNOWN), param1(0), param2(0), pParam3(nullptr){}
		TWindowMessage(E_WINDOW_MESSAGE_TYPE msg, uint32 param1 = 0, uint32 param2 = 0, void *param3 = nullptr) :messageType(msg), param1(param1), param2(param2), pParam3(param3){}
	};
#if defined(PLATFORM_WINDOWS) && defined(STRUCT_ALIGN_1)
#pragma pack(pop)
#endif

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
		EIF_FULLSCREEN			= 0x00000001,
		EIF_NATIVE_RESOLUTION	= 0x00000002,
		EIF_NO_LOG				= 0x00000004
	};

	class ICore : public ITGE_Base
	{
	public:
		virtual HRESULT CALLBACK InitializeEngine(uint resX, uint resY, const char *appName, E_ENGINE_INIT_FLAGS initFlags) = 0;
		virtual HRESULT CALLBACK QuitEngine() = 0;
		virtual HRESULT CALLBACK AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK AddToLog(const char *txt, bool isError) = 0;

		virtual void TestDelegates() = 0;
	};
	
	extern"C"
	{
		__declspec(dllexport) bool GetEngine(TGE::ICore *& pICore);
		__declspec(dllexport) void FreeEngine();
	}
}


#endif //_TGE_H_