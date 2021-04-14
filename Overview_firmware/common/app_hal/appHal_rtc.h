#ifndef _APPHAL_RTC_H_
#define _APPHAL_RTC_H_

#include "nrf_rtc.h"
#include "nrf_drv_rtc.h"


void appHal_rtc_config(void);
uint32_t appHal_rtc_reset_counter(void);
uint32_t appHal_rtc_elapsed_ms(void);
uint32_t appHal_rtc_elapsed_since_ms(uint32_t baseTime);

#endif