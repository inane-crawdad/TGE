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

#ifdef PLATFORM_WINDOWS

typedef HWND WindowHandle;

#else

#endif

#include "TGE_CommonTypes.h"

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

#ifdef _MSC_VER
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#else
	typedef long long int64;
	typedef unsigned long long uint64;
#endif

	// main interface of engine
	class ITGE_Base{};


	// types of messages
	enum E_WINDOW_MESSAGE_TYPE
	{
		WMT_UNKNOWN = 0,
		WMT_CLOSE,
		WMT_DESTROY,
		WMT_REDRAW,
		WMT_ACTIVATED,
		WMT_DEACTIVATED,
		WMT_MOVE,
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

	class IInput;

	class ICore : public ITGE_Base
	{
	public:
		virtual HRESULT CALLBACK InitializeEngine(uint resX, uint resY, const char *appName, E_ENGINE_INIT_FLAGS initFlags) = 0;
		virtual HRESULT CALLBACK QuitEngine() = 0;
		virtual HRESULT CALLBACK AddFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK RemoveFunction(E_FUNC_TYPE funcType, void(CALLBACK *func)(void *pParam), void *pParam = nullptr) = 0;
		virtual HRESULT CALLBACK AddToLog(const char *txt, bool isError) = 0;

		virtual HRESULT CALLBACK GetInput(IInput *&pInput) = 0;
	};

	enum E_KEYBOARD_KEY_CODES;

#ifdef PLATFORM_WINDOWS

#if defined(PLATFORM_WINDOWS) && defined(STRUCT_ALIGN_1)
#pragma pack(push,1)
#endif
	struct TMouseState
	{
		int X;
		int Y;
		int deltaX;
		int deltaY;
		int deltaWheel;
		bool lButtonPressed;
		bool rButtonPressed;
		bool mButtonPressed;
	};
#if defined(PLATFORM_WINDOWS) && defined(STRUCT_ALIGN_1)
#pragma pack(pop)
#endif

	enum E_INPUT_CONFIGURATION_FLAGS
	{
		ICF_DEFAULT				= 0x00000000,
		ICF_EXCLUSIVE			= 0x00000001,
		ICF_HIDE_CURSOR			= 0x00000002,
		ICF_ALWAYS_IN_CENTER	= 0x00000004
	};

	class IInput : public ITGE_Base
	{
	public:
		virtual HRESULT CALLBACK Configure(E_INPUT_CONFIGURATION_FLAGS flags = ICF_DEFAULT) = 0;
		virtual HRESULT CALLBACK GetKey(E_KEYBOARD_KEY_CODES eKey, bool &isPressed) = 0;
		virtual HRESULT CALLBACK GetMouseState(TMouseState& mouseState) = 0;
	};

	enum E_KEYBOARD_KEY_CODES : uint8
	{
		KEY_ESCAPE = 0x01,
		KEY_TAB = 0x0F,
		KEY_GRAVE = 0x29,
		KEY_CAPSLOCK = 0x3A,
		KEY_BACKSPACE = 0x0E,
		KEY_RETURN = 0x1C,
		KEY_SPACE = 0x39,
		KEY_SLASH = 0x35,
		KEY_BACKSLASH = 0x2B,

		KEY_SYSRQ = 0xB7,
		KEY_SCROLL = 0x46,
		KEY_PAUSE = 0xC5,

		KEY_INSERT = 0xD2,
		KEY_DELETE = 0xD3,
		KEY_HOME = 0xC7,
		KEY_END = 0xCF,
		KEY_PGUP = 0xC9,
		KEY_PGDN = 0xD1,

		KEY_LSHIFT = 0x2A,
		KEY_RSHIFT = 0x36,
		KEY_LALT = 0x38,
		KEY_RALT = 0xB8,
		KEY_LWIN_OR_CMD = 0xDB,
		KEY_RWIN_OR_CMD = 0xDC,
		KEY_LCONTROL = 0x1D,
		KEY_RCONTROL = 0x9D,

		KEY_UP = 0xC8,
		KEY_RIGHT = 0xCD,
		KEY_LEFT = 0xCB,
		KEY_DOWN = 0xD0,

		KEY_1 = 0x02,
		KEY_2 = 0x03,
		KEY_3 = 0x04,
		KEY_4 = 0x05,
		KEY_5 = 0x06,
		KEY_6 = 0x07,
		KEY_7 = 0x08,
		KEY_8 = 0x09,
		KEY_9 = 0x0A,
		KEY_0 = 0x0B,

		KEY_F1 = 0x3B,
		KEY_F2 = 0x3C,
		KEY_F3 = 0x3D,
		KEY_F4 = 0x3E,
		KEY_F5 = 0x3F,
		KEY_F6 = 0x40,
		KEY_F7 = 0x41,
		KEY_F8 = 0x42,
		KEY_F9 = 0x43,
		KEY_F10 = 0x44,
		KEY_F11 = 0x57,
		KEY_F12 = 0x58,

		KEY_Q = 0x10,
		KEY_W = 0x11,
		KEY_E = 0x12,
		KEY_R = 0x13,
		KEY_T = 0x14,
		KEY_Y = 0x15,
		KEY_U = 0x16,
		KEY_I = 0x17,
		KEY_O = 0x18,
		KEY_P = 0x19,
		KEY_A = 0x1E,
		KEY_S = 0x1F,
		KEY_D = 0x20,
		KEY_F = 0x21,
		KEY_G = 0x22,
		KEY_H = 0x23,
		KEY_J = 0x24,
		KEY_K = 0x25,
		KEY_L = 0x26,
		KEY_Z = 0x2C,
		KEY_X = 0x2D,
		KEY_C = 0x2E,
		KEY_V = 0x2F,
		KEY_B = 0x30,
		KEY_N = 0x31,
		KEY_M = 0x32,

		KEY_MINUS = 0x0C,
		KEY_PLUS = 0x0D,
		KEY_LBRACKET = 0x1A,
		KEY_RBRACKET = 0x1B,

		KEY_SEMICOLON = 0x27,
		KEY_APOSTROPHE = 0x28,

		KEY_COMMA = 0x33,
		KEY_PERIOD = 0x34,

		KEY_NUMPAD0 = 0x52,
		KEY_NUMPAD1 = 0x4F,
		KEY_NUMPAD2 = 0x50,
		KEY_NUMPAD3 = 0x51,
		KEY_NUMPAD4 = 0x4B,
		KEY_NUMPAD5 = 0x4C,
		KEY_NUMPAD6 = 0x4D,
		KEY_NUMPAD7 = 0x47,
		KEY_NUMPAD8 = 0x48,
		KEY_NUMPAD9 = 0x49,
		KEY_NUMPADPERIOD = 0x53,
		KEY_NUMPADENTER = 0x9C,
		KEY_NUMPADSTAR = 0x37,
		KEY_NUMPADPLUS = 0x4E,
		KEY_NUMPADMINUS = 0x4A,
		KEY_NUMPADSLASH = 0xB5,
		KEY_NUMLOCK = 0x45,
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