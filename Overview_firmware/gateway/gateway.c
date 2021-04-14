#include "gateway.h"

sensors_t mockSS;
gen_fifo_t thread_rx_fifo;
fifo_data_packet_t thread_rx_fifo_buf[PRE_UART_TX_BUF_SIZE];

//  888    888        d8888 888b    888 8888888b.  888      8888888888 8888888b.  
//  888    888       d88888 8888b   888 888  "Y88b 888      888        888   Y88b 
//  888    888      d88P888 88888b  888 888    888 888      888        888    888 
//  8888888888     d88P 888 888Y88b 888 888    888 888      8888888    888   d88P 
//  888    888    d88P  888 888 Y88b888 888    888 888      888        8888888P"  
//  888    888   d88P   888 888  Y88888 888    888 888      888        888 T88b   
//  888    888  d8888888888 888   Y8888 888  .d88P 888      888        888  T88b  
//  888    888 d88P     888 888    Y888 8888888P"  88888888 8888888888 888   T88b 

void bsp_event_handler(uint32_t button, uint32_t timePushed)
{
    // char* fake_str = "d040404040404g";
    // thread_msg_t thread_msg;

    if (timePushed > 30000)
    {
        bsp_notif();
    }
    else
    {
        bsp_notif();
    }
    SEGGER_RTT_printf(0, "UART_NOTIF_LEN %d \n\r", UART_NOTIF_LEN);
    // parse_message(fake_str, &thread_msg);
    // send_notif(&thread_msg);
    send_fake_notif();
}

//  8888888 888b    888 8888888 88888888888 
//    888   8888b   888   888       888     
//    888   88888b  888   888       888     
//    888   888Y88b 888   888       888     
//    888   888 Y88b888   888       888     
//    888   888  Y88888   888       888     
//    888   888   Y8888   888       888     
//  8888888 888    Y888 8888888     888  

void init (void)
{
    appHal_init_wdt();
    log_init();
    print_banner();
    scheduler_init();
    // timer_init();
    gateway_bsp_init(bsp_event_handler);
    gen_fifo_init(&thread_rx_fifo, thread_rx_fifo_buf, PRE_UART_TX_BUF_SIZE, 
                        PRE_UART_TX_ELMT_SIZE, sizeof(thread_rx_fifo_buf));
    app_thread_gateway_init(&thread_rx_fifo);
    app_uart_gateway_init();
    #if(MOCK_UART_DATA)
        init_mock_uart_data();
        init_mock();
    #endif
}

void print_banner(void)
{
    SEGGER_RTT_printf(0, "\n");
    SEGGER_RTT_printf(0, "************************************************************** \n");
    SEGGER_RTT_printf(0, "*                                                            * \n");
    SEGGER_RTT_printf(0, "*                    GATEWAY STARTED !                       * \n");
    SEGGER_RTT_printf(0, "*                                                            * \n");
    SEGGER_RTT_printf(0, "************************************************************** \n");
    nrf_delay_ms(10);
}

// // Function for initializing the Application Timer Module.
// void timer_init(void)
// {
//     uint32_t error_code = app_timer_init();
//     APP_ERROR_CHECK(error_code);
// }

// Function for initializing the nrf log module.
void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

// Function for initializing scheduler module.
void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

// 88888888888 888    888 8888888b.  8888888888        d8888 8888888b.  
//     888     888    888 888   Y88b 888              d88888 888  "Y88b 
//     888     888    888 888    888 888             d88P888 888    888 
//     888     8888888888 888   d88P 8888888        d88P 888 888    888 
//     888     888    888 8888888P"  888           d88P  888 888    888 
//     888     888    888 888 T88b   888          d88P   888 888    888 
//     888     888    888 888  T88b  888         d8888888888 888  .d88P 
//     888     888    888 888   T88b 8888888888 d88P     888 8888888P"  

void gateway_thread_process(void)
{
    if (gen_fifo_length(&thread_rx_fifo)) 
    {
        send_data_gateway();
    }
}

// 888     888        d8888 8888888b. 88888888888 
// 888     888       d88888 888   Y88b    888     
// 888     888      d88P888 888    888    888     
// 888     888     d88P 888 888   d88P    888     
// 888     888    d88P  888 8888888P"     888     
// 888     888   d88P   888 888 T88b      888     
// Y88b. .d88P  d8888888888 888  T88b     888     
//  "Y88888P"  d88P     888 888   T88b    888     

void uart_process (void)
{
    thread_msg_t thread_msg;
    if (uart_gateway_process(&thread_msg))
    {
        send_notif(&thread_msg);
    }
}

void send_data_gateway (void)
{
    fifo_data_packet_t data_from_fifo;
    gen_fifo_read(&thread_rx_fifo, &data_from_fifo);
    uart_tx_mesh_packet(&data_from_fifo);
}

//  88b      d888        d8888 8888888 888b    888
//  8888b   d8888       d88888   888   8888b   888
//  88888b.d88888      d88P888   888   88888b  888
//  888Y88888P888     d88P 888   888   888Y88b 888
//  888 Y888P 888    d88P  888   888   888 Y88b888
//  888  Y8P  888   d88P   888   888   888  Y88888
//  888   "   888  d8888888888   888   888   Y8888
//  888       888 d88P     888 8888888 888    Y888

int main(int argc, char * argv[])
{
    init();
    
    while (true)
    {
        uart_process();
        gateway_thread_process();
        thread_process();
        app_sched_execute();
        appHal_feed_the_dog();
    }
}