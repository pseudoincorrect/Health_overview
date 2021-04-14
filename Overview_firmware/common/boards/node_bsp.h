#if !defined(__NODE_BSP_H__)
#define __NODE_BSP_H__

#include <stdint.h>
#include "app_button.h"
#include "app_timer.h"
#include "SEGGER_RTT.h"
#include "node_board.h"
#include "nrf_delay.h"
#include "appHal_gpio.h"

typedef void (*buttonCallback_t)(uint32_t delay);

static void button_handler(uint8_t button, uint8_t action);
void buttons_init(buttonCallback_t callback);
void gpio_output_init(void);
void notification_from_base(void);
static void timer_handler(void * p_context);

#endif // __NODE_BSP_H__
