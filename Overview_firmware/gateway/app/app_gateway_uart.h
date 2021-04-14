#if !defined(__APP_UART_GATEWAY_H__)
#define __APP_UART_GATEWAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "command_def.h"
#include "gateway_board.h"
#include "SEGGER_RTT.h"
#include "nrf_gpio.h"
#include "app_uart.h"
#include "print_utils.h"
#if defined (UART_PRESENT)
    #include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
    #include "nrf_uarte.h"
#endif

#define UART_HWFC                 APP_UART_FLOW_CONTROL_DISABLED
#define UART_TX_BUF_SIZE          256
#define UART_RX_BUF_SIZE          256
#define UART_PRINTF               (printf)
#define PRE_UART_TX_BUF_SIZE      (8)
#define PRE_UART_TX_ELMT_SIZE     (sizeof(fifo_data_packet_t))

void extract_addr_from_str(char* received_str, node_addr_t* addr);
void parse_message(char* received_str, thread_msg_t* thread_msg);
bool process_rx_uart_message(char* received_str, thread_msg_t* thread_msg);
void uart_error_handle(app_uart_evt_t* p_event);
void app_uart_gateway_init(void);
bool uart_gateway_process(thread_msg_t* thread_msg);
uint32_t uart_rx_check(char* str_target);
void uart_tx_mesh_packet(fifo_data_packet_t* data_from_fifo);
void fill_notif_thread_msg(thread_msg_t* thread_msg, uint8_t type, uint8_t content, node_addr_t* addr);
bool message_empty(char* received_str);
bool message_valid(char* received_str);

#endif // __APP_UART_H__