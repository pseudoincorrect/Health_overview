#ifndef  _WATCHDOG_h_
#define _WATCHDOG_h_

///////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
///////////////////////////////////////////////////////////////////////////////

#include "nrf_drv_wdt.h"
#include "nrf_drv_clock.h"
#include "app_timer.h"

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////

void init_wdt (void);
void feed_the_dog (void);

#endif