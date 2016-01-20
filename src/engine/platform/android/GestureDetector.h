#ifndef _GESTUREDETECTOR_H_
#define _GESTUREDETECTOR_H_

#include "../PlatformAPI.h"

class CTapDetector : public IGestureDetector, private TGE::CUncopyable
{
	struct _TDownTime
	{
		int id;
		TGE::int64 time;
		float x, y;
	};

	TGE::uint32 _tapTimeout = 180 * 1000000;
	std::vector<_TDownTime> _downIDs;
	std::vector<TGE::TScreenPoint> _taps;
	TGE::uint16 _tapSlop = 8;

public:
	void Configure(const TGE::TTouchParams& touchParams);

	HRESULT GetGestureState(TGE::TGestureState& gestureState) override final;
	HRESULT Detect(const TGE::TWindowMessage &msg) override final;
	HRESULT UpdateState() override final;
};

class CDoubleTapDetector : public IGestureDetector, private TGE::CUncopyable
{
	struct _TDownTime
	{
		int id;
		TGE::int64 time;
		float x, y;
	};

	struct _TTap
	{
		TGE::int64 time;
		float x, y;
	};

	TGE::uint32 _tapTimeout = 180 * 1000000;
	TGE::uint32 _doubleTapTimeout = 300 * 1000000;
	std::vector<_TDownTime> _downIDs;
	std::vector<_TTap> _taps;
	std::vector<TGE::TScreenPoint> _doubleTaps;
	TGE::uint16 _tapSlop = 8;
	TGE::uint16 _doubleTapSlop = 100;

public:
	void Configure(const TGE::TTouchParams& touchParams);

	HRESULT GetGestureState(TGE::TGestureState& gestureState) override final;
	HRESULT Detect(const TGE::TWindowMessage &msg) override final;
	HRESULT UpdateState() override final;
};

class CDragDetector : public IGestureDetector, private TGE::CUncopyable
{
protected:
	std::vector<TGE::TTouchPointer> _touchPointers;

public:
	HRESULT GetGestureState(TGE::TGestureState& gestureState) override;
	HRESULT Detect(const TGE::TWindowMessage &msg) override final;
	HRESULT UpdateState() override final;
};

class CPinchDetector : public CDragDetector
{
public:
	HRESULT GetGestureState(TGE::TGestureState& gestureState) override final;
};

#endif //_GESTUREDETECTOR_H_