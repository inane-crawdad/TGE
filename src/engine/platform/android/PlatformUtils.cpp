#include "..\..\Common.h"

TWindowMessage AndroidMsgToEngMsg(E_ANDROID_EVENT_TYPE eventType, AInputEvent *event, int cmd)
{
	switch (eventType)
	{
	case AET_CMD:
		switch (cmd)
		{
		case APP_CMD_INIT_WINDOW:
			return TWindowMessage(WMT_CREATE);
			break;
		case APP_CMD_TERM_WINDOW:
			return TWindowMessage(WMT_RELEASED);
			break;
		case APP_CMD_GAINED_FOCUS:
			return TWindowMessage(WMT_ACTIVATED);
			break;
		case APP_CMD_LOST_FOCUS:
			return TWindowMessage(WMT_DEACTIVATED);
			break;
		case APP_CMD_DESTROY:
			return TWindowMessage(WMT_CLOSE);
			break;
		default:
			return TWindowMessage();
			break;
		}
		break;
	case AET_INPUT:
		return TWindowMessage(WMT_TOUCH_EVENT, 0, 0, static_cast<void*>(event));
		break;
	}
}

uint64 GetPerfTimer()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void GetLocalSysTime(TSysTime &time)
{
	time_t t = ::time(NULL);
	struct tm tm = *localtime(&t);
	struct timeval tv;
	gettimeofday(&tv, NULL);

	time.year = tm.tm_year + 1900;
	time.month = tm.tm_mon + 1;
	time.day = tm.tm_mday;
	time.hour = tm.tm_hour;
	time.minute = tm.tm_min;
	time.second = tm.tm_sec;
	time.milliSecond = tv.tv_usec / 1000;
}

void GetDisplaySize(uint &width, uint &height)
{
	//don't really need it
}

void Terminate()
{
	std::terminate();
}