#ifndef __COMMON_DEBUG_H__
#define __COMMON_DEBUG_H__

#include <iostream>

#include "Clock.h"

#define dout_prefix ""

#define dout std::cout << clock_now() << ' ' << dout_prefix
#define dendl std::endl;

#endif /* common/Debug.h */
