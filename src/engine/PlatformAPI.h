#ifndef _PLATFORM_API_
#define _PLATFORM_API_

#include "TGE.h"
#include "FuncDelegate.h"

class IMainWindow
{
public:
	virtual HRESULT InitWindow(TProcDelegate *procDelegate, TMsgProcDelegate *msgProcDelegate) = 0;
	virtual HRESULT ConfigureWindow(TGE::uint resX, TGE::uint resY, bool fullScreen) = 0;
	virtual HRESULT SetCaption(const char *pCaption) = 0;
	virtual HRESULT GetClientRect(int32 &left, int32 &right, int32 &top, int32 &bottom) = 0;
	virtual HRESULT BeginMainLoop() = 0;
	virtual HRESULT KillWindow() = 0;
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

uint64 GetPerfTimer();
void GetLocalSysTime(TSysTime &time);
void Terminate();


#endif //_PLATFORM_API_