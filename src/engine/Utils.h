#ifndef _UTILS_H_
#define _UTILS_H_

#include "TGE.h"

#ifdef TGE_DESKTOP
TGE::uchar EngKeyToASCIIKey(const TGE::uint8 key);
TGE::uint8 ASCIIKeyToEngKey(const TGE::uchar key);
#endif

#endif