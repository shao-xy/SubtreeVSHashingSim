#ifndef __COMMON_DEBUG_H__
#define __COMMON_DEBUG_H__

#include <iostream>

#include "Clock.h"

#define dout std::cout << clock_now() << ' '
#define dendl std::endl;

#endif /* common/Debug.h */
