#ifndef _INPUT_H_
#define _INPUT_H_

#include "../Common.h"

class CInput : public IInput, private CUncopyable
{
	CCore			*_pCore;
	IPlatformInput	*_pPlatformInput;

	IGestureDetector *_pTapDetector;
	IGestureDetector *_pDoubleTapDetector;
	IGestureDetector *_pDragDetector;
	IGestureDetector *_pPinchDetector;


	void _ClearDetectors();

	void _MsgProc(const TWindowMessage& msg);
	static void CALLBACK _s_MsgProc(const TWindowMessage& msg, void *pParam);

	void _Process();
	static void CALLBACK _s_Process(void *pParam);

public:

	CInput(CCore *pCore);
	~CInput();

	HRESULT CALLBACK Configure(E_GESTURE_TYPE flags, const TTouchParams& touchParams) override final;
	HRESULT CALLBACK GetGestureState(E_GESTURE_TYPE gestureType, TGestureState& gestureState) override final;

	HRESULT CALLBACK GetSubsystemType(E_ENGINE_SUB_SYSTEM &subsystemType) override final;
};

#endif