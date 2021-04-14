#ifndef __NODE_H__
#define __NODE_H__

///////////////////////////////////////////////////////////////////////////////
//    Included Files
///////////////////////////////////////////////////////////////////////////////
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
// App Hal
#include "appHal_gpio.h"
#include "appHal_rtc.h"
// Middleware
#include "watchdog.h"
#include "print_utils.h"
#include "node_mock.h"
#include "print_utils.h"
// Board
#include "node_board.h"
#include "node_bsp.h"
// App
#include "app_sensors.h"
#include "app_node_thread.h"

///////////////////////////////////////////////////////////////////////////////
//    Definitions
///////////////////////////////////////////////////////////////////////////////

#define RTT_INPUT_POLL_PERIOD_MS (100)
#define SCHED_QUEUE_SIZE        32
#define SCHED_EVENT_DATA_SIZE   APP_TIMER_SCHED_EVENT_DATA_SIZE
#define TX_BUFF_THRESHOLD       5


#define HIGH_POWER_MODE 1
#define LOW_POWER_MODE 0

enum power_mode {HIGH_POWER, LOW_POWER};

typedef struct {
    enum power_mode mode;
    uint32_t time_activated;
} power_mode_t;

///////////////////////////////////////////////////////////////////////////////
//    Function Declarations
///////////////////////////////////////////////////////////////////////////////
void bsp_event_handler(uint32_t timePushed);
static inline bool can_goto_sleep(void);
static inline bool can_transmit_something(void);
void init_power_mode(void);
void init(void);
bool is_low_power_mode(void);
bool is_time_for_low_power_mode(void);
void log_init(void);
void main_unblocking_process(void);
int main(int argc, char * argv[]);
void node_bsp_init(void);
void rx_thread(thread_msg_t* threadMsg_p);
void scheduler_init(void);
void set_high_power_mode(void);
void set_low_power_mode(void);
void utils_timer_init(void);
void send_notification(uint8_t type);

#endif
