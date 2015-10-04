#include "..\..\Common.h"

TWindowMessage WinAPIMsgToEngMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case(WM_CLOSE) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_CLOSE);
		break;
	case(WM_DESTROY) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_DESTROY);
		break;
	case(WM_PAINT) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_REDRAW);
		break;
	case(WM_KEYDOWN) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_KEY_DOWN, ASCIIKeyToEngKey((TGE::uchar)wParam));
		break;
	case(WM_KEYUP):
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_KEY_UP, ASCIIKeyToEngKey((TGE::uchar)wParam));
		break;
	case(WM_MOUSEMOVE):
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_MOVE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case(WM_LBUTTONDOWN):
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_DOWN, 0);
		break;
	case(WM_RBUTTONDOWN) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_DOWN, 1);
		break;
	case(WM_MBUTTONDOWN) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_DOWN, 2);
		break;
	case(WM_LBUTTONUP) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_UP, 0);
		break;
	case(WM_RBUTTONUP) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_UP, 1);
		break;
	case(WM_MBUTTONUP) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_UP, 2);
		break;
	case(WM_MOUSEWHEEL) :
		return TGE::TWindowMessage(TGE::E_WINDOW_MESSAGE_TYPE::WMT_MOUSE_WHEEL, GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		return TWindowMessage();
		break;
	}
}
void EngMsgToWinAPIMsg(const TWindowMessage &engMsg, UINT &Msg, WPARAM &wParam, LPARAM &lParam)
{

}

uint64 GetPerfTimer()
{
	LARGE_INTEGER time, freq;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&freq);
	return (1000000 * time.QuadPart / freq.QuadPart);
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