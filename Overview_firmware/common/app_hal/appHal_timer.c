#include "appHal_timer.h"

void appHal_timerInit (const nrf_drv_timer_t* timer_instance, uint32_t freq, void * timer_cb)
{
    uint32_t time_ms = freq; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(timer_instance, &timer_cfg, timer_cb);
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(timer_instance, time_ms);

    nrf_drv_timer_extended_compare(
        timer_instance, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrf_drv_timer_enable(timer_instance);
}