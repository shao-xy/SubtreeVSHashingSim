#ifndef __COMMON_DEBUG_H__
#define __COMMON_DEBUG_H__

#include <iostream>

#include "Clock.h"
#include "ExternalLogger.h"

#define dout_prefix ""

#define cout std::cout << clock_now() << ' ' << dout_prefix
#define dout gdout << clock_now() << ' ' << dout_prefix
#define dendl std::endl;

#endif /* common/Debug.h */
