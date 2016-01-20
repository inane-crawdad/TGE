#include "GestureDetector.h"
#include <cmath>

void CTapDetector::Configure(const TGE::TTouchParams& touchParams)
{
	_tapTimeout = touchParams.tapTimeout * 1000000;
	_tapSlop	= touchParams.tapSlop;
}

HRESULT CTapDetector::GetGestureState(TGE::TGestureState& gestureState)
{
	gestureState.points = _taps;
	return S_OK;
}

HRESULT CTapDetector::Detect(const TGE::TWindowMessage &msg)
{
	auto motion_event = static_cast<AInputEvent*>(msg.pParam3);
	if (motion_event == nullptr) return E_FAIL;

	auto action = AMotionEvent_getAction(motion_event);
	auto flag = action & AMOTION_EVENT_ACTION_MASK;
	auto index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) 
		>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	switch (flag)
	{
	case AMOTION_EVENT_ACTION_DOWN:
	case AMOTION_EVENT_ACTION_POINTER_DOWN:
	{
		_TDownTime temp;
		temp.id = AMotionEvent_getPointerId(motion_event, index);
		temp.time = AMotionEvent_getEventTime(motion_event);
		temp.x = AMotionEvent_getX(motion_event, index);
		temp.y = AMotionEvent_getY(motion_event, index);
		_downIDs.push_back(temp);
		break;
	}	
	case AMOTION_EVENT_ACTION_UP:
	case AMOTION_EVENT_ACTION_POINTER_UP:
		auto id = AMotionEvent_getPointerId(motion_event, index);
		TGE::int64 downTime = 0;
		float oldX, oldY;
		for (auto it = _downIDs.begin(); it < _downIDs.end(); ++it)
			if (it->id == id)
			{
				downTime = it->time;
				oldX = it->x;
				oldY = it->y;
				_downIDs.erase(it);
				break;
			}
		if (downTime != 0 && AMotionEvent_getEventTime(motion_event) - downTime <= _tapTimeout)
		{
			auto x = AMotionEvent_getX(motion_event, index);
			auto y = AMotionEvent_getY(motion_event, index);
			auto deltaX = oldX - x;
			auto deltaY = oldY - y;
			if (deltaX * deltaX + deltaY * deltaY <= _tapSlop * _tapSlop)
			{
				TGE::TScreenPoint temp;
				temp.x = x;
				temp.y = y;
				_taps.push_back(temp);
			}
		}

		if (flag == AMOTION_EVENT_ACTION_UP || flag == AMOTION_EVENT_ACTION_CANCEL)
			_downIDs.clear();

		break;
	}

	return S_OK;
}

HRESULT CTapDetector::UpdateState()
{
	_taps.clear();
	return S_OK;
}



void CDoubleTapDetector::Configure(const TGE::TTouchParams& touchParams)
{
	_tapTimeout			= touchParams.tapTimeout * 1000000;
	_doubleTapTimeout	= touchParams.doubleTapTimeout * 1000000;
	_tapSlop			= touchParams.tapSlop;
	_doubleTapSlop		= touchParams.doubleTapSlop;
}

HRESULT CDoubleTapDetector::GetGestureState(TGE::TGestureState& gestureState)
{
	gestureState.points = _doubleTaps;
	return S_OK;
}

HRESULT CDoubleTapDetector::Detect(const TGE::TWindowMessage &msg)
{
	auto motion_event = static_cast<AInputEvent*>(msg.pParam3);
	if (motion_event == nullptr) return E_FAIL;

	auto action = AMotionEvent_getAction(motion_event);
	auto flag = action & AMOTION_EVENT_ACTION_MASK;
	auto index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
		>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	switch (flag)
	{
	case AMOTION_EVENT_ACTION_DOWN:
	case AMOTION_EVENT_ACTION_POINTER_DOWN:
	{
		auto time = AMotionEvent_getEventTime(motion_event);

		_TDownTime temp;
		temp.id = AMotionEvent_getPointerId(motion_event, index);
		temp.time = time;
		temp.x = AMotionEvent_getX(motion_event, index);
		temp.y = AMotionEvent_getY(motion_event, index);
		_downIDs.push_back(temp);

		for (auto it = _taps.begin(); it < _taps.end(); )
		{
			if (time - it->time > _doubleTapTimeout)
			{
				it = _taps.erase(it);
				continue;
			}

			auto x = AMotionEvent_getX(motion_event, index);
			auto y = AMotionEvent_getY(motion_event, index);
			if (pow(x - it->x, 2) + pow(y - it->y, 2) <= pow(_doubleTapSlop, 2))
			{
				TGE::TScreenPoint temp;
				temp.x = x;
				temp.y = y;
				_doubleTaps.push_back(temp);

				it = _taps.erase(it);
				continue;
			}
			

			++it;
		}

		break;
	}
	case AMOTION_EVENT_ACTION_UP:
	case AMOTION_EVENT_ACTION_POINTER_UP:
		auto id = AMotionEvent_getPointerId(motion_event, index);
		TGE::int64 downTime = 0;
		float oldX, oldY;
		for (auto it = _downIDs.begin(); it < _downIDs.end(); ++it)
			if (it->id == id)
			{
				downTime = it->time;
				oldX = it->x;
				oldY = it->y;
				_downIDs.erase(it);
				break;
			}
		if (downTime != 0 && AMotionEvent_getEventTime(motion_event) - downTime <= _tapTimeout)
		{
			auto x = AMotionEvent_getX(motion_event, index);
			auto y = AMotionEvent_getY(motion_event, index);
			auto deltaX = oldX - x;
			auto deltaY = oldY - y;
			if (deltaX * deltaX + deltaY * deltaY <= _tapSlop * _tapSlop)
			{
				_TTap temp;
				temp.x = x;
				temp.y = y;
				temp.time = downTime;
				_taps.push_back(temp);
			}
		}

		if (flag == AMOTION_EVENT_ACTION_UP || flag == AMOTION_EVENT_ACTION_CANCEL)
			_downIDs.clear();

		break;
	}
	return S_OK;
}

HRESULT CDoubleTapDetector::UpdateState()
{
	_doubleTaps.clear();
	return S_OK;
}




HRESULT CDragDetector::GetGestureState(TGE::TGestureState& gestureState)
{
	gestureState.touchPointers = _touchPointers;
	return S_OK;
}

HRESULT CDragDetector::Detect(const TGE::TWindowMessage &msg)
{
	auto motion_event = static_cast<AInputEvent*>(msg.pParam3);
	if (motion_event == nullptr) return E_FAIL;

	auto action = AMotionEvent_getAction(motion_event);
	auto flag = action & AMOTION_EVENT_ACTION_MASK;
	auto index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
		>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
	
	switch (flag)
	{
	case AMOTION_EVENT_ACTION_DOWN:
	case AMOTION_EVENT_ACTION_POINTER_DOWN:
	{
		TGE::TTouchPointer temp;
		TGE::TScreenPoint tempPoint;
		temp.id = AMotionEvent_getPointerId(motion_event, index);
		tempPoint.x = AMotionEvent_getX(motion_event, index);
		tempPoint.y = AMotionEvent_getY(motion_event, index);
		temp.oldPoint = tempPoint;
		temp.currentPoint = tempPoint;
		_touchPointers.push_back(temp);
		break;
	}
	case AMOTION_EVENT_ACTION_UP:
	case AMOTION_EVENT_ACTION_POINTER_UP:
	{
		auto id = AMotionEvent_getPointerId(motion_event, index);
		for (auto it = _touchPointers.begin(); it < _touchPointers.end(); ++it)
			if (it->id == id)
			{
				_touchPointers.erase(it);
				break;
			}
		break;
	}
	case AMOTION_EVENT_ACTION_MOVE:
	{
		auto count = AMotionEvent_getPointerCount(motion_event);
		for (int i = 0; i < count; ++i)
		{
			auto id = AMotionEvent_getPointerId(motion_event, i);
			for (auto it = _touchPointers.begin(); it < _touchPointers.end(); ++it)
				if (it->id == id)
				{
					it->currentPoint.x = AMotionEvent_getX(motion_event, i);
					it->currentPoint.y = AMotionEvent_getY(motion_event, i);
					break;
				}
		}

		break;
	}

	}

	if (flag == AMOTION_EVENT_ACTION_UP || flag == AMOTION_EVENT_ACTION_CANCEL)
		_touchPointers.clear();

	return S_OK;
}

HRESULT CDragDetector::UpdateState()
{
	for (auto touchPointer : _touchPointers)
		touchPointer.oldPoint = touchPointer.currentPoint;

	return S_OK;
}



HRESULT CPinchDetector::GetGestureState(TGE::TGestureState& gestureState)
{
	gestureState.touchPointers.clear();
	if (_touchPointers.size() >= 2u)
	{
		gestureState.touchPointers.push_back(_touchPointers[0]);
		gestureState.touchPointers.push_back(_touchPointers[1]);
	}
	
	return S_OK;
}

