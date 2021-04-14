#if !defined(__GATEWAY_MOCK_H__)
#define __GATEWAY_MOCK_H__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 

#include "nrf_drv_timer.h"
#include "nrf.h"
#include "app_gateway_uart.h"
#include "command_def.h"

void mock_uart_data (void);
void init_mock_uart_data (void);
void get_mock_uart_data (char* msg_str_raw);
void init_mock(void);

#endif // __GATEWAY_MOCK_H__
