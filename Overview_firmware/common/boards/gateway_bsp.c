#ifndef  _NODE_BSP_H_
#define  _NODE_BSP_H_

#include "gateway_bsp.h"

#define BUTTON_DETECTION_DELAY    APP_TIMER_TICKS(50)

buttonCallback_t m_callback;

static uint32_t button_tick = 0;
static uint32_t button_tick_release = 0;
static uint32_t total_ticks = 0;

APP_TIMER_DEF(m_button_tick_timer);

void gateway_bsp_init (buttonCallback_t callback)
{
    buttons_init(callback);
	gpio_output_init();
}

static void button_handler(uint8_t button, uint8_t action)
{
	uint32_t err_code;
	switch(action) {
	case APP_BUTTON_PUSH:
		app_timer_start(m_button_tick_timer, 65535, NULL);
		button_tick = app_timer_cnt_get();
		break;
	case APP_BUTTON_RELEASE:
        button_tick_release = app_timer_cnt_get();

		err_code = app_timer_stop(m_button_tick_timer);
		APP_ERROR_CHECK(err_code);

		total_ticks = app_timer_cnt_diff_compute(button_tick_release, button_tick);
		// SEGGER_RTT_printf(0, "total ticks was: %d \n", total_ticks);

        m_callback(button, total_ticks);
	break;
	}
}

static void timer_handler(void * p_context)
{
	UNUSED_PARAMETER(p_context);
	SEGGER_RTT_printf(0, "timer timed out \n");
}

void buttons_init(buttonCallback_t callback)
{
	// Note: Array must be static because a pointer to it will be saved 
	// in the Button handler module.
	uint32_t err_code = 0;

    m_callback = callback;

	static app_button_cfg_t buttons[] = {
		{
				PIN_BUTTON_1,
				APP_BUTTON_ACTIVE_LOW,
				NRF_GPIO_PIN_PULLUP,
				button_handler
		},
		{
				PIN_BUTTON_2,
				APP_BUTTON_ACTIVE_LOW,
				NRF_GPIO_PIN_PULLUP,
				button_handler
		},
		{
				PIN_BUTTON_3,
				APP_BUTTON_ACTIVE_LOW,
				NRF_GPIO_PIN_PULLUP,
				button_handler
		},
		{
				PIN_BUTTON_4,
				APP_BUTTON_ACTIVE_LOW,
				NRF_GPIO_PIN_PULLUP,
				button_handler
		},
	};

	SEGGER_RTT_printf(0, " button = %d\n\r", sizeof(buttons)/sizeof(app_button_cfg_t));
	err_code = app_button_init((app_button_cfg_t *) (&buttons),
	                           sizeof(buttons)/sizeof(app_button_cfg_t),
	                           BUTTON_DETECTION_DELAY);
	APP_ERROR_CHECK(err_code);
	err_code = app_button_enable();
	APP_ERROR_CHECK(err_code);

	err_code = app_timer_create(&m_button_tick_timer, APP_TIMER_MODE_REPEATED, timer_handler);
}

void gpio_output_init (void)
{
	appHal_simple_gpio_init(PIN_LED_1, OUTPUT_DIR);
    appHal_simple_gpio_init(PIN_LED_2, OUTPUT_DIR);
    appHal_simple_gpio_init(PIN_LED_3, OUTPUT_DIR);
    appHal_simple_gpio_init(PIN_LED_4, OUTPUT_DIR);

    appHal_write(PIN_LED_1, 1);
    appHal_write(PIN_LED_2, 1);
    appHal_write(PIN_LED_3, 1);
    appHal_write(PIN_LED_4, 1);

	bsp_notif();
}

void bsp_notif (void)
{
    for (int i=0; i<3; i++)
    {
    	appHal_write(PIN_LED_1, 0);
        nrf_delay_ms(200);
    	appHal_write(PIN_LED_1, 1);
        nrf_delay_ms(200);
    }
}

#endif