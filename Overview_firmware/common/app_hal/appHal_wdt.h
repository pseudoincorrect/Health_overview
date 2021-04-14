#if !defined(__APPHAL_WDT_H__)
#define __APPHAL_WDT_H__

#include <stdint.h>
#include "nrf_drv_wdt.h"
#include "nrf_drv_clock.h"
#include "app_timer.h"

static void wdt_event_handler(void);
void appHal_feed_the_dog (void);
void appHal_init_wdt (void);

#endif // __APPHAL_WDT_H__
