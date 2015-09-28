#ifndef _COMMON_H_
#define _COMMON_H_

#include "TGE.h"
using namespace TGE;

#include "FuncDelegate.h"
#include "PlatformAPI.h"

#ifdef PLATFORM_WINDOWS
#include "GL\glew.h"
#include "GL\wglew.h"
#endif

// std libs
#include <iostream>
#include <fstream>


#endif // _COMMON_H_