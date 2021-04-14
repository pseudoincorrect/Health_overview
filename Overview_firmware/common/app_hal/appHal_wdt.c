#include "appHal_wdt.h"



nrf_drv_wdt_channel_id m_channel_id;

static void wdt_event_handler(void)
{
    ;
}

void appHal_init_wdt (void) 
{
    uint32_t err_code = NRF_SUCCESS;

    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request(NULL);

    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_wdt_config_t config = NRF_DRV_WDT_DEAFULT_CONFIG;
    err_code = nrf_drv_wdt_init(&config, wdt_event_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_wdt_channel_alloc(&m_channel_id);
    APP_ERROR_CHECK(err_code);
    nrf_drv_wdt_enable();
}

void appHal_feed_the_dog (void)
{
    nrf_drv_wdt_channel_feed(m_channel_id);
}
