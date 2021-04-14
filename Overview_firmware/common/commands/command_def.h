#ifndef __COMMAND_DEF_H__
#define __COMMAND_DEF_H__

#include "stdint.h"
#include "stdbool.h"
#include "thread_msg_types.h"

#define NOP do { __asm__ __volatile_S_ ("nop"); } while (0);

// UART commands
// #define G_N_UPDATE_REQ   'a'
// #define G_N_NOTIF_ALL    'b'
// #define G_N_NOTIF_SETUP  'c'
// #define G_N_NOTIF_REST   'd'
// #define N_G_NOTIF        'e'
// #define N_G_UPDATE       'f'
// #define N_G_GATEWAY_REQ  'g'
// #define G_N_GATEWAY_RES  'h'

// UART msg_type
#define G_N_NOTIF   'a' 
#define N_G_NOTIF   'b' 
#define N_G_SENSORS 'c' 
#define R_G_SENSORS 'd' 
#define G_R_SETUP   'e' 
#define R_G_SETUP   'f'

// notif content
#define NODE_SETUP  'a'

#define UART_NOTIF_LEN     (2 * sizeof(char) * sizeof(node_addr_t) \
                                + sizeof(notif_t) + sizeof(msg_type_t))
#define UART_NOTIF_CONTENT_POSITION (UART_NOTIF_LEN - 1)
#define UART_TYPE_POSITION 0

#define ERROR_UART       'y'
#define PORT_TEST_MSG    'z'

#define RECV_ADDR_SIZE (ADDR_LEN*2)

// position of the command char in a "sigle cmd" message
#define CMD_CHAR_POS  (ADDR_LEN+1)

// ammount of heartrate sample to average (mean)
#define SAMPLE_SIZE 5

// size of the thread message (TX) ring buffer
// MUST BE a power of 2
#define THREAD_BUFF_SIZE 64

#define HIGH_POWER_MODE_DURATION 30000

#define SENSORS_MOCK 0

typedef thread_msg_t fifo_data_packet_t;

#endif

