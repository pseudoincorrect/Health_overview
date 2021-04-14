
#if !defined(__GATEWAY_BSP_H__)
#define __GATEWAY_BSP_H__

#include <stdint.h>
#include "app_button.h"
#include "app_timer.h"
#include "SEGGER_RTT.h"
#include "gateway_board.h"
#include "nrf_delay.h"
#include "appHal_gpio.h"

typedef void (*buttonCallback_t)(uint32_t button, uint32_t delay);

void gateway_bsp_init (buttonCallback_t callback);
void buttons_init(buttonCallback_t callback);
void gpio_output_init(void);
static void timer_handler(void * p_context);
void bsp_notif (void);

#endif // __GATEWAY_BSP_H__
