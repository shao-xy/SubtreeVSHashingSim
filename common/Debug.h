#ifndef __COMMON_DEBUG_H__
#define __COMMON_DEBUG_H__

#include <iostream>

#include "Clock.h"
#include "ExternalLogger.h"

#define dout_prefix ""

#define cout_impl (std::cout)

#define cout_flush cout_impl.flush()

#define cout cout_impl << clock_now() << ' ' << dout_prefix
#define dout gdout << clock_now() << ' ' << dout_prefix
using std::endl;
#define dendl std::endl;

#endif /* common/Debug.h */
