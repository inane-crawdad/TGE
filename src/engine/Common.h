#ifndef _COMMON_H_
#define _COMMON_H_

#include "TGE.h"
using namespace TGE;

class CCore;

#include "FuncDelegate.h"
#include "platform\PlatformAPI.h"
#include "platform\PlatformSubsystemManager.h"
#include "Utils.h"

#ifdef TGE_DESKTOP
#include "GL/glew.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "GL/wglew.h"
#endif

#ifdef PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <time.h>
#include <exception>
#include <android/log.h>
#endif

// std libs
//#include <iostream>
#include <fstream>



#include "Core.h"

#endif // _COMMON_H_