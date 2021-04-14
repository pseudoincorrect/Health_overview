#include "appHal_gpio.h"

static uint32_t m_last_button_press;
static hal_button_handler_cb_t m_button_handler_cb;

uint32_t appHal_write (uint32_t pin_nb, uint32_t  value)
{
    if (value)
        nrf_gpio_pin_write(pin_nb, 1);
    else
        nrf_gpio_pin_write(pin_nb, 0);

    return value;
}

int appHal_read (uint32_t pin_nb)
{
    return nrf_gpio_pin_read(pin_nb);
}

void appHal_simple_gpio_init (uint32_t pin, uint32_t direction)
{
    if (direction == OUTPUT_DIR)
        nrf_gpio_cfg_output(pin);
    else
        nrf_gpio_cfg_input(pin, NRF_GPIO_PIN_PULLUP);
}

void appHal_toggle(uint32_t pin_nb)
{
    nrf_gpio_pin_toggle(pin_nb);
}


void appHal_blink(uint32_t pin_nb, uint32_t delay_ms, uint32_t repeat, uint8_t endState)
{
    repeat &= 0xFE;

    repeat = repeat*2+2; // get the right amount of blinks

    for (uint32_t i = 0; i < repeat; ++i)
    {
        if (i & ((uint32_t) 1 << 0))
            appHal_write(pin_nb, 0);
        else
            appHal_write(pin_nb, 1);

        nrf_delay_ms(delay_ms);
    }
    appHal_write(pin_nb, endState);
}

void appHal_in_int_init(uint8_t pin, void* callback)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config =  {
        .is_watcher = true,
        .hi_accuracy = true,
        .pull = NRF_GPIO_PIN_PULLUP,
        .sense = NRF_GPIOTE_POLARITY_TOGGLE
    };

    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(pin, &in_config, callback);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(pin, true);
}
