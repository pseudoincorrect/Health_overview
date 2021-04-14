#if !defined(__GATEWAY_H__)
#define __GATEWAY_H__

// standard inc
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <openthread/thread.h>
// SEGGER RTT (debug print)
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "SEGGER_RTT.h"
// nrf drivers
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_uart.h"
#include "nrf_uarte.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_wdt.h"
#include "nrf_drv_clock.h"
// app midleware (common)
#include "app_scheduler.h"
#include "app_timer.h"
#include "app_uart.h"
#include "app_error.h"
// thread midle ware
#include "thread_coap_utils.h"
#include "thread_utils.h"
// general files
#include "gen_fifo.h"
#include "print_utils.h"
#include "command_def.h"
// app gateway specific
#include "app_gateway_thread.h"
#include "app_gateway_uart.h"
#include "gateway_mock.h"
// app Hardware Abstraction Layer
#include "appHal_wdt.h"
#include "gateway_bsp.h"

#define RTT_INPUT_POLL_PERIOD_MS  (100)
#define SCHED_QUEUE_SIZE          32  
#define SCHED_EVENT_DATA_SIZE     APP_TIMER_SCHED_EVENT_DATA_SIZE 
#define MOCK_UART_DATA            0

void bsp_event_handler(uint32_t button, uint32_t timePushed);
void print_banner(void);
void feed_the_dog(void);
void gateway_thread_process(void);
void init_mock(void);
void init_wdt(void);
void log_complete_process(void);
void log_init(void);
int main(int argc, char * argv[]);
void mock_timer_handler(nrf_timer_event_t event_type, void* p_context);
void mock_uart_data(void);
void rx_thread(thread_msg_t* thread_msg);
void scheduler_init(void);
void send_data_gateway(void);
void set_msg_addr(node_addr_t* addr, node_addr_t* msg_addr);
void thread_coap_init(void);
void thread_instance_init(void);
void thread_state_changed_callback(uint32_t flags, void * p_context);
void timer_init(void);
void uart_process(void);
void wdt_event_handler(void);

#endif // __GATEWAY_H__
