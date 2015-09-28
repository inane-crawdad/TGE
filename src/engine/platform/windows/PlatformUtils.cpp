#include "..\..\Common.h"

TWindowMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	default:
		return TWindowMessage();
		break;
	}
}
void EngMsgToWinAPIMsg(const TWindowMessage &engMsg, UINT &Msg, WPARAM &wParam, LPARAM &lParam)
{

}


void GetLocalSysTime(TSysTime &time)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	time.year = st.wYear;
	time.month = st.wMonth;
	time.day = st.wDay;
	time.hour = st.wHour;
	time.minute = st.wMinute;
	time.second = st.wSecond;
	time.milliSecond = st.wMilliseconds;
}

void Terminate()
{
	TerminateProcess(GetCurrentProcess(), -1);
}