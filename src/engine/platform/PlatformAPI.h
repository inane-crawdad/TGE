#ifndef _PLATFORM_API_
#define _PLATFORM_API_

#include "..\..\..\include\TGE.h"
#include "..\FuncDelegate.h"

enum E_PLATFORM_SUB_SYSTEM_TYPE
{
	PSST_MAIN_WINDOW = 0,
	PSST_INPUT,
	PSST_RENDER
};

class IPlatformSubsystem
{
	virtual HRESULT GetPlatformSubsystemType(E_PLATFORM_SUB_SYSTEM_TYPE &pst) = 0;
};

class IMainWindow : public IPlatformSubsystem
{
public:
	virtual HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) = 0;
	virtual HRESULT ConfigureWindow(const TGE::TWindowParams &winParams) = 0;
	virtual HRESULT SetCaption(const char *pCaption) = 0;
	virtual HRESULT GetClientRect(TGE::int32 &left, TGE::int32 &right, TGE::int32 &top, TGE::int32 &bottom) = 0;
	virtual HRESULT GetWindowHandle(TGE::WindowHandle &winHandle) = 0;
	virtual HRESULT GetWindowDrawContext(TGE::WindowDrawContext &dcHandle) = 0;
	virtual HRESULT BeginMainLoop() = 0;
	virtual HRESULT KillWindow() = 0;
	virtual HRESULT Free() = 0;
};

class IPlatformRender : public IPlatformSubsystem
{
public:
	virtual HRESULT Initialize(const TGE::TWindowParams &winParams) = 0;
	virtual HRESULT Free() = 0;
	virtual HRESULT MakeCurrent() = 0;
};

class IPlatformInput : public IPlatformSubsystem
{
public:
	virtual HRESULT GetCursorPos(int &x, int &y) = 0;
	virtual HRESULT SetCursorPos(int x, int y) = 0;
	virtual HRESULT ShowCursor(bool show) = 0;
	virtual HRESULT ClipCursor(TGE::int32 left, TGE::int32 right, TGE::int32 top, TGE::int32 bottom) = 0;
	virtual HRESULT Free() = 0;
};

struct TSysTime
{
	TGE::uint16 year;
	TGE::uint16 month;
	TGE::uint16 day;
	TGE::uint16 hour;
	TGE::uint16 minute;
	TGE::uint16 second;
	TGE::uint16 milliSecond;
};

#ifdef PLATFORM_WINDOWS
TGE::TWindowMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
void EngMsgToWinAPIMsg(const TGE::TWindowMessage &engMsg, UINT &Msg, WPARAM &wParam, LPARAM &lParam);
#else

#endif

TGE::uint64 GetPerfTimer();
void GetLocalSysTime(TSysTime &time);
void GetDisplaySize(TGE::uint &width, TGE::uint &height);
void Terminate();


#endif //_PLATFORM_API_