#ifndef __APP_NODE_THREAD_H__
#define __APP_NODE_THREAD_H__

#include <stdint.h>
#include <stdbool.h>
// SDK OpenThread
#include <openthread/instance.h>
#include <openthread/thread.h>
// Thread abstraction

#include "app_scheduler.h"
#include "thread_coap_utils.h"
#include "thread_utils.h"
// SDK app libraries
#include "appHal_rtc.h"
#include "app_timer.h"
#include "nrf.h"
#include "bsp_thread.h" // need to be integrated to appHal
#include "command_def.h"
#include "SEGGER_RTT.h"
#include "print_utils.h"

#define NUM_SLAAC_ADDRESSES    4  // Number of SLAAC addresses
#define CONN_TIMEOUT 15000

typedef void (*rx_thread_callback_t) (thread_msg_t* threadMsg_p);
typedef void (*buttonCallback_t) (uint32_t delay);

typedef struct
{
    thread_msg_t data[THREAD_BUFF_SIZE];
    uint8_t     w_ptr; 
    uint8_t     r_ptr;
    bool        send_next;
    bool        retry;
    int         timeout;
} tx_msg_buffer_t;

typedef struct
{
    bool            is_connected;
    bool            start_connecting;
    int             start_time;
    thread_role_t   current_role;
    node_addr_t     node_addr;
    otNetifAddress  m_slaac_addresses[NUM_SLAAC_ADDRESSES];
} connection_state_t;

typedef struct
{
    tx_msg_buffer_t     buff;
    connection_state_t  conn;
} app_thread_t;


void app_rx_thread(thread_msg_t* threadMsg_p);
void app_thread_deinit(void);
app_thread_t* app_thread_get_data_struct(void);
void app_thread_init_data_struct(void);
void app_thread_init (rx_thread_callback_t rx_thread_callback, thread_role_t role);
void thread_instance_init(thread_role_t role);
void app_thread_push_msg(thread_msg_t* thread_msg);
int buffer_fill(void);
static inline void change_role(thread_role_t role);
void check_connection();
bool compare_addr(uint8_t* p_addr);
static inline void disconnect(void);
static inline bool is_buffer_empty(void);
bool is_connected(void);
static inline bool is_connection_already_started(void);
static inline bool is_connection_timeout(void);
static inline bool is_current_role_different(thread_role_t role);
static inline bool is_in_mesh_network(void);
static inline bool is_msg_sent_with_success(void);
static inline bool is_no_server(void);
static inline bool is_send_timeout(void);
static inline bool is_something_to_queue(void);
static inline void let_thread_do_stuff(void);
static inline void queue_msg(void);
static inline void retry_send(void);
void rx_callback_init(rx_thread_callback_t rx_thread_callback);
static inline void select_next_msg(void);
void set_addr(void);
void set_rx_state(thread_role_t role);
static inline void start_connection(void);
static inline void stop_trying_connect(void);
void thread_coap_init(void);
void thread_send_process(void);
void thread_state_changed_callback(uint32_t flags, void * p_context);
void tx_thread(thread_msg_t* threadMsg_p);
node_addr_t* get_addr(void);

#endif