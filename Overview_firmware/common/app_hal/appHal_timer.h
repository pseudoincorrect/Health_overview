#ifndef MYHAL_TIMER_H
#define MYHAL_TIMER_H

#include "nrf_delay.h"
#include "nrf_timer.h"
#include "node_board.h"
#include "SEGGER_RTT.h"
#include "nrf.h"
#include "nrf_drv_timer.h"

void appHal_timerInit (const nrf_drv_timer_t* timer_instance, uint32_t freq, void * timer_cb);

#endif
