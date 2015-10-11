#ifndef _COMMON_H_
#define _COMMON_H_

#include "TGE.h"
using namespace TGE;

class CCore;

#include "FuncDelegate.h"
#include "platform\PlatformAPI.h"
#include "platform\PlatformSubsystemManager.h"
#include "Utils.h"
#include "GL/glew.h"

#ifdef PLATFORM_WINDOWS
#include "GL/wglew.h"
#endif

// std libs
#include <iostream>
#include <fstream>



#include "Core.h"

#endif // _COMMON_H_