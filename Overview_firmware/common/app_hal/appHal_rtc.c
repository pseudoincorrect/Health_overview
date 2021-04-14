//8888888b. 88888888888  .d8888b.
//888   Y88b    888     d88P  Y88b
//888    888    888     888    888
//888   d88P    888     888
//8888888P"     888     888
//888 T88b      888     888    888
//888  T88b     888     Y88b  d88P
//888   T88b    888      "Y8888P"

#include "appHal_rtc.h"

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0);


static void appHal_rtc_handler(nrf_drv_rtc_int_type_t int_type) {}

void appHal_rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = 32; // around 1ms
    err_code = nrf_drv_rtc_init(&rtc, &config,  appHal_rtc_handler);
    APP_ERROR_CHECK(err_code);
    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}

uint32_t appHal_rtc_reset_counter(void)
{
    return NRF_RTC0->TASKS_CLEAR = 1;
}

uint32_t appHal_rtc_elapsed_ms(void)
{
    return (NRF_RTC0->COUNTER * 32000 / 32768);
}

uint32_t appHal_rtc_elapsed_since_ms(uint32_t baseTime)
{
    return (uint32_t) (appHal_rtc_elapsed_ms() - baseTime);
}

