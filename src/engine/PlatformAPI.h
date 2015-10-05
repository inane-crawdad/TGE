#ifndef _PLATFORM_API_
#define _PLATFORM_API_

#include "TGE.h"
#include "FuncDelegate.h"

class IMainWindow
{
public:
	virtual HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) = 0;
	virtual HRESULT ConfigureWindow(uint resX, uint resY, bool fullScreen) = 0;
	virtual HRESULT SetCaption(const char *pCaption) = 0;
	virtual HRESULT GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) = 0;
	virtual HRESULT GetWindowHandle(WindowHandle &winHandle) = 0;
	virtual HRESULT BeginMainLoop() = 0;
	virtual HRESULT KillWindow() = 0;
	virtual HRESULT Free() = 0;
};

struct TSysTime
{
	uint16 year;
	uint16 month;
	uint16 day;
	uint16 hour;
	uint16 minute;
	uint16 second;
	uint16 milliSecond;
};

#ifdef PLATFORM_WINDOWS
TGE::TWindowMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam);
void EngMsgToWinAPIMsg(const TGE::TWindowMessage &engMsg, UINT &Msg, WPARAM &wParam, LPARAM &lParam);
#else

#endif

uint64 GetPerfTimer();
void GetLocalSysTime(TSysTime &time);
void GetDisplaySize(uint &width, uint &height);
void Terminate();


#endif //_PLATFORM_API_