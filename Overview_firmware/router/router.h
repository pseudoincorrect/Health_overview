#ifndef MAIN_H
#define MAIN_H

///////////////////////////////////////////////////////////////////////////////
//    Included Files
///////////////////////////////////////////////////////////////////////////////
// SDK OpenThread
#include <openthread/instance.h>
#include <openthread/thread.h>

// Thread abstraction
#include "thread_coap_utils.h"
#include "thread_utils.h"
// SDK Nrf Driver
#include "nrf_drv_timer.h"
#include "nrf_drv_clock.h"
// SDK Log
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "SEGGER_RTT.h"
// SDK app libraries
#include "app_scheduler.h"
#include "app_timer.h"
#include "nrf.h"
#include "bsp_thread.h" // need to be integrated to appHal
// App Hal
#include "appHal_gpio.h"
#include "appHal_rtc.h"
// Middleware
#include "watchdog.h"
#include "print_utils.h"
#include "print_utils.h"
// Board
#include "node_board.h"

///////////////////////////////////////////////////////////////////////////////
//    Definitions
///////////////////////////////////////////////////////////////////////////////

#define RTT_INPUT_POLL_PERIOD_MS (100)
#define segger_printf(...)       SEGGER_RTT_printf(0, __VA_ARGS__)
#define segger_println(...)      { SEGGER_RTT_printf(0, __VA_ARGS__); \
                                    SEGGER_RTT_printf(0, "\n\r"); }
#define SCHED_QUEUE_SIZE         32
#define SCHED_EVENT_DATA_SIZE    APP_TIMER_SCHED_EVENT_DATA_SIZE

///////////////////////////////////////////////////////////////////////////////
//    Function Prototypes
///////////////////////////////////////////////////////////////////////////////

void reach_gateway (void);
uint16_t array_mean_16(uint16_t* array, uint8_t size);
uint8_t array_mean_8(uint8_t* array, uint8_t size);
void bsp_event_handler(bsp_event_t event);
int compare_addr(uint8_t* p_addr);
void init_gpio(void);
void input_output_process(void);
void log_init(void);
void rx_thread(thread_msg_t* p_threadMsg);
void scheduler_init(void);
void sensors_thread_process(void);
void* set_addr(void);
void thread_bsp_init(void);
void thread_coap_init(void);
void thread_instance_init(void);
void thread_send_process(void);
void thread_state_changed_callback(uint32_t flags, void * p_context);
void timer_handler(nrf_timer_event_t event_type, void* p_context);
void tx_thread(thread_msg_t* p_threadMsg);
void update_timer_init(void);
void utils_timer_init(void);
void notification_from_base (void);

#endif
