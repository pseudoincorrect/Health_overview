
#include "app_gateway_uart.h"

int32_t str_target_iter;
int str_complete;
char received_str_g[UART_RX_BUF_SIZE];


//  888    888        d8888 888b    888 8888888b.  888      8888888888 8888888b.  
//  888    888       d88888 8888b   888 888  "Y88b 888      888        888   Y88b 
//  888    888      d88P888 88888b  888 888    888 888      888        888    888 
//  8888888888     d88P 888 888Y88b 888 888    888 888      8888888    888   d88P 
//  888    888    d88P  888 888 Y88b888 888    888 888      888        8888888P"  
//  888    888   d88P   888 888  Y88888 888    888 888      888        888 T88b   
//  888    888  d8888888888 888   Y8888 888  .d88P 888      888        888  T88b  
//  888    888 d88P     888 888    Y888 8888888P"  88888888 8888888888 888   T88b 

// Uart error handler
void uart_error_handle(app_uart_evt_t* p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

//  8888888 888b    888 8888888 88888888888 
//    888   8888b   888   888       888     
//    888   88888b  888   888       888     
//    888   888Y88b 888   888       888     
//    888   888 Y88b888   888       888     
//    888   888  Y88888   888       888     
//    888   888   Y8888   888       888     
//  8888888 888    Y888 8888888     888  

void app_uart_gateway_init (void)
{
    str_target_iter = 0;
    memset(received_str_g, 0, sizeof(received_str_g));
    str_complete = 0;
    uint32_t err_code;

    const app_uart_comm_params_t comm_params = {
        RX_PIN_NUMBER,
        TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        UART_HWFC,
        false,
        #if defined (UART_PRESENT)
        NRF_UART_BAUDRATE_115200
        #else
        NRF_UARTE_BAUDRATE_115200
        #endif
    };

    APP_UART_FIFO_INIT(
        &comm_params,
        UART_RX_BUF_SIZE,
        UART_TX_BUF_SIZE,
        uart_error_handle,
        APP_IRQ_PRIORITY_LOWEST,
        err_code);

    APP_ERROR_CHECK(err_code);
}


void uart_tx_mesh_packet (fifo_data_packet_t* data_from_fifo) 
{    
    char msg_str_raw[64];
    if (data_from_fifo->type == N_G_SENSORS)
    {
        sprintf(msg_str_raw,
            "%04x%02x%02x%02x%02x%02x%02x%04x%04x%04x%04x%04x%04x%04x",
            N_G_SENSORS,
            data_from_fifo->content.sensors.addr.a[0],
            data_from_fifo->content.sensors.addr.a[1],
            data_from_fifo->content.sensors.addr.a[2],
            data_from_fifo->content.sensors.addr.a[3],
            data_from_fifo->content.sensors.addr.a[4],
            data_from_fifo->content.sensors.addr.a[5],
            data_from_fifo->content.sensors.origin.node.temperature,
            data_from_fifo->content.sensors.origin.node.heart_rate,
            data_from_fifo->content.sensors.origin.node.oxymetry,
            data_from_fifo->content.sensors.origin.node.humidity,
            data_from_fifo->content.sensors.origin.node.dehydration,
            data_from_fifo->content.sensors.origin.node.reserved1,
            data_from_fifo->content.sensors.origin.node.reserved2);
    }
    else if (data_from_fifo->type == N_G_NOTIF)
    {
        sprintf(msg_str_raw,
            "%04x%02x%02x%02x%02x%02x%02x%02x",
            N_G_NOTIF,
            data_from_fifo->content.notif.addr.a[0],
            data_from_fifo->content.notif.addr.a[1],
            data_from_fifo->content.notif.addr.a[2],
            data_from_fifo->content.notif.addr.a[3],
            data_from_fifo->content.notif.addr.a[4],
            data_from_fifo->content.notif.addr.a[5],
            data_from_fifo->content.notif.data);
    }
    SEGGER_RTT_printf(0, "%s\n", msg_str_raw);
    UART_PRINTF("%s\n", msg_str_raw);
} 

                       
uint32_t uart_rx_check(char* str_target) 
{
    uint8_t cr;

    if (app_uart_get(&cr) == NRF_SUCCESS) {
        if (str_target_iter < UART_RX_BUF_SIZE) {
            if (isalnum(cr)) {
                str_target[str_target_iter++] = cr;
            }
            else if ((cr == 8) || (cr == 127) || (cr == 177)) { // \177 = BS
                if (str_target_iter > 0) { // check underflow
                    str_target[--str_target_iter] = 0x00;
                }
            }
            else if (cr == '\r') {
               str_target[++str_target_iter] = 0x00; // set last char to \0
               str_target_iter = 0;
               str_complete = 1;
            }           
            else {
               str_complete = 0;
            }
        }
        else {
            str_target_iter = 0;
        }
    }
    return 1;
}

bool process_rx_uart_message(char* received_str, thread_msg_t* thread_msg) 
{
    uint32_t nrf_status = NRF_SUCCESS;
    uint8_t msg_type;

    if (message_empty(received_str)) { return false; }


    msg_type = received_str[UART_TYPE_POSITION];
    
    SEGGER_RTT_printf(0, "Message received from PC: \"%s\" \n\r", received_str);

    if (msg_type == PORT_TEST_MSG) { 
        // used here to recogniser the NRF by the host computer
        UART_PRINTF("yes\n");
    }
    else if (msg_type == G_N_NOTIF) {
        // UART_PRINTF("\n\rreceived_str: \"%s\"\n", received_str);
        SEGGER_RTT_printf(0, "G_N_NOTIF \n\r");
        SEGGER_RTT_printf(0, "message valid ? %d \n\r", message_valid(received_str));
        if (message_valid(received_str)) 
        {
            parse_message(received_str, thread_msg);
            return true;
        }
    }
    else
    {
       UART_PRINTF("%c\n", msg_type);
    }
    return false;
}

// 888    888 8888888888 888      8888888b.  8888888888 8888888b.  
// 888    888 888        888      888   Y88b 888        888   Y88b 
// 888    888 888        888      888    888 888        888    888 
// 8888888888 8888888    888      888   d88P 8888888    888   d88P 
// 888    888 888        888      8888888P"  888        8888888P"  
// 888    888 888        888      888        888        888 T88b   
// 888    888 888        888      888        888        888  T88b  
// 888    888 8888888888 88888888 888        8888888888 888   T88b 

bool message_empty(char* received_str)
{
    return (strlen(received_str) == 0);
}

bool message_valid(char* received_str)
{
    return (strlen(received_str) == UART_NOTIF_LEN);
}


void parse_message (char* received_str, thread_msg_t* thread_msg)
{
    node_addr_t addr;
    extract_addr_from_str(received_str, &addr);
    fill_notif_thread_msg(
        thread_msg,
        (uint8_t) G_N_NOTIF,
        received_str[UART_NOTIF_CONTENT_POSITION],
        &addr 
    );
    print_pretty_nofif_thread_msg(thread_msg);
}

void extract_addr_from_str (char* received_str, node_addr_t* addr)
{
    char msg_addr_MSB[9];
    char msg_addr_LSB[5];
    unsigned int addr_l_MSB;
    unsigned int addr_l_LSB;
    // ex of received msg "s4734cb0b8fe3"
    // copy the MSB of the addr to a temp str
    memcpy(msg_addr_MSB, &received_str[1], 8);
    msg_addr_MSB[8] = 0; // set the end of string to the null value
    // copy the LSB of the addr to a temp str
    memcpy(msg_addr_LSB, &received_str[9], 4);
    msg_addr_LSB[4] = 0; // set the end of string to the null value
    // convert the MSB/LSB string to double and place in the long buffer
    addr_l_MSB = strtoul(msg_addr_MSB, NULL, 16);
    addr_l_LSB = strtoul(msg_addr_LSB, NULL, 16);
    // copy the double (type) value of the MSB addr to advertiser buffer
    addr->a[0] = addr_l_MSB >> 24;
    addr->a[1] = addr_l_MSB >> 16;
    addr->a[2] = addr_l_MSB >> 8;
    addr->a[3] = addr_l_MSB;
    // copy the double (type) value of the LSB addr to advertiser buffer
    addr->a[4] = addr_l_LSB >> 8;
    addr->a[5] = addr_l_LSB;
}

void fill_notif_thread_msg(thread_msg_t* thread_msg, 
                        uint8_t type, uint8_t content, node_addr_t* addr)
{
    thread_msg->type = G_N_NOTIF;
    thread_msg->content.notif.data = content;
    memcpy(&thread_msg->content.notif.addr, addr, sizeof(node_addr_t));
}

// 8888888b.  8888888b.   .d88888b.   .d8888b.  8888888888  .d8888b.   .d8888b.  
// 888   Y88b 888   Y88b d88P" "Y88b d88P  Y88b 888        d88P  Y88b d88P  Y88b 
// 888    888 888    888 888     888 888    888 888        Y88b.      Y88b.      
// 888   d88P 888   d88P 888     888 888        8888888     "Y888b.    "Y888b.   
// 8888888P"  8888888P"  888     888 888        888            "Y88b.     "Y88b. 
// 888        888 T88b   888     888 888    888 888              "888       "888 
// 888        888  T88b  Y88b. .d88P Y88b  d88P 888        Y88b  d88P Y88b  d88P 
// 888        888   T88b  "Y88888P"   "Y8888P"  8888888888  "Y8888P"   "Y8888P"  
                                                                              
bool uart_gateway_process (thread_msg_t* thread_msg)
{
    bool is_thread_message = false;
    uart_rx_check(received_str_g);

    if (str_complete == 1) {
        is_thread_message = process_rx_uart_message(received_str_g, thread_msg);
        memset(received_str_g, 0, sizeof(received_str_g));
        str_complete = 0;
    };
    
    return is_thread_message;
}