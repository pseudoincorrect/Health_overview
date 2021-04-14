#if !defined(__APP_GATEWAY_THREAD_H__)
#define __APP_GATEWAY_THREAD_H__

#include <openthread/thread.h>
#include "command_def.h"
#include "print_utils.h"
#include "thread_coap_utils.h"
#include "gen_fifo.h"

void app_thread_gateway_init (gen_fifo_t* rx_fifo);
bool compare_addr(uint8_t* addr_1, uint8_t* addr_2);
void gateway_thread_process(void);
void reach_gateway_res(void);
void rx_thread(thread_msg_t* thread_msg);
void send_fake_notif(void);
void send_notif(thread_msg_t* thread_msg);
void set_msg_addr(node_addr_t* addr, node_addr_t* msg_addr);
void thread_coap_init(void);
void thread_instance_init(void);
void thread_state_changed_callback(uint32_t flags, void * p_context);

#endif // __APP_GATEWAY_THREAD_H__
