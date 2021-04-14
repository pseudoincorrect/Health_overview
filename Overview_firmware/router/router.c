////////////////////////////////////////////////////////////////////////////////
//    Included Files
////////////////////////////////////////////////////////////////////////////////

#include "router.h"

////////////////////////////////////////////////////////////////////////////////
//    Define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////

const nrf_drv_timer_t timerLed_g = NRF_DRV_TIMER_INSTANCE(2);
node_addr_t routerAddr_g;
node_addr_t gatewayAddr_g;
bool valid_gateway_g;
uint32_t base_gateway_search_time_g;
//  Buffer containing addresses resolved by SLAAC
#define NUM_SLAAC_ADDRESSES      4  /**< Number of SLAAC addresses. */
static otNetifAddress m_slaac_addresses[NUM_SLAAC_ADDRESSES];     

////////////////////////////////////////////////////////////////////////////////
//    Functions
////////////////////////////////////////////////////////////////////////////////



//  888    888        d8888 888b    888 8888888b.  888      8888888888 8888888b.
//  888    888       d88888 8888b   888 888  "Y88b 888      888        888   Y88b
//  888    888      d88P888 88888b  888 888    888 888      888        888    888
//  8888888888     d88P 888 888Y88b 888 888    888 888      8888888    888   d88P
//  888    888    d88P  888 888 Y88b888 888    888 888      888        8888888P"
//  888    888   d88P   888 888  Y88888 888    888 888      888        888 T88b
//  888    888  d8888888888 888   Y8888 888  .d88P 888      888        888  T88b
//  888    888 d88P     888 888    Y888 8888888P"  88888888 8888888888 888   T88b

////////////////////////////////////////////////////////////////////////////////
// 
//
void bsp_event_handler(bsp_event_t event)
{

    valid_gateway_g = false;
    bsp_board_led_on(3);
    base_gateway_search_time_g = appHal_rtc_elapsed_ms();

    //    switch (event)
    //    {
    //        case BSP_EVENT_KEY_0:
    //            break;
    //        case BSP_EVENT_KEY_1:
    //            break;
    //        case BSP_EVENT_KEY_2:
    //            break;
    //        case BSP_EVENT_KEY_3:
    //            // thread_coap_utils_provisioning_request_send(thread_ot_instance_get());
    //            break;
    //        default:
    //            return; // no implementation needed
    //    }
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void thread_state_changed_callback(uint32_t flags, void * p_context)
{
    if (flags & OT_CHANGED_THREAD_ROLE)
    {
        switch(otThreadGetDeviceRole(p_context))
        {
            case OT_DEVICE_ROLE_CHILD:
            case OT_DEVICE_ROLE_ROUTER:
            case OT_DEVICE_ROLE_LEADER:
                break;

            case OT_DEVICE_ROLE_DISABLED:
            case OT_DEVICE_ROLE_DETACHED:
            default:
            thread_coap_utils_peer_addr_clear();
                break;
        }
        NRF_LOG_INFO("OT_CHANGED_THREAD_ROLE changed ! Flags: 0x%08x Current role: %d\r\n", flags, otThreadGetDeviceRole(p_context));

    }

    if (flags & OT_CHANGED_THREAD_PARTITION_ID)
    {
        thread_coap_utils_peer_addr_clear();
        NRF_LOG_INFO("OT_CHANGED_THREAD_PARTITION_ID changed !");
    }

    if (flags & OT_CHANGED_THREAD_NETDATA)
    {
        /**
         * Whenever Thread Network Data is changed, it is necessary to check if generation of global
         * addresses is needed. This operation is performed internally by the OpenThread CLI module.
         * To lower power consumption, the examples that implement Thread Sleepy End Device role
         * don't use the OpenThread CLI module. Therefore otIp6SlaacUpdate util is used to create
         * IPv6 addresses.
         */
        otIp6SlaacUpdate(thread_ot_instance_get(), m_slaac_addresses,
                         sizeof(m_slaac_addresses) / sizeof(m_slaac_addresses[0]),
                         otIp6CreateRandomIid, NULL);
        NRF_LOG_INFO("OT_CHANGED_THREAD_NETDATA changed !");
    }

}

////////////////////////////////////////////////////////////////////////////////
// 
//
void timer_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            feed_the_dog();
            break;
        default:
            break;
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

////////////////////////////////////////////////////////////////////////////////
// 
//
void thread_bsp_init(void)
{
    uint32_t error_code = bsp_init(
            BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(error_code);

    error_code = bsp_thread_init(thread_ot_instance_get());
    APP_ERROR_CHECK(error_code);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void utils_timer_init(void)
{
    uint32_t error_code = app_timer_init();
    APP_ERROR_CHECK(error_code);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void update_timer_init(void)
{
    uint32_t time_ms = 5000;
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&timerLed_g, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);
    time_ticks = nrf_drv_timer_ms_to_ticks(&timerLed_g, time_ms);
    nrf_drv_timer_extended_compare(
        &timerLed_g, NRF_TIMER_CC_CHANNEL0, time_ticks, 
        NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&timerLed_g);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .role              = RX_ON_WHEN_IDLE,
        .autocommissioning = true,
        .poll_period           = 2500,
        .default_child_timeout = 10,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void thread_coap_init(void)
{
    thread_coap_configuration_t thread_coap_configuration =
    {
        .coap_server_enabled               = true, 
        .coap_client_enabled               = true,
        .coap_cloud_enabled                = false,
        .configurable_led_blinking_enabled = false,
    };

    thread_coap_utils_init(&thread_coap_configuration);

    enable_provisionning(false);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

//  8888888b.  Y88b   d88P     88888888888 888    888 8888888b.
//  888   Y88b  Y88b d88P          888     888    888 888   Y88b
//  888    888   Y88o88P           888     888    888 888    888
//  888   d88P    Y888P            888     8888888888 888   d88P
//  8888888P"     d888b            888     888    888 8888888P"
//  888 T88b     d88888b           888     888    888 888 T88b
//  888  T88b   d88P Y88b          888     888    888 888  T88b
//  888   T88b d88P   Y88b         888     888    888 888   T88b

////////////////////////////////////////////////////////////////////////////////
// 
//
void rx_thread(thread_msg_t* p_threadMsg)
{
    if (p_threadMsg->command != N_G_UPDATE)
    {
        segger_println("Thread RX");
        print_pretty_threadMsg(p_threadMsg);
        if (p_threadMsg->command == G_N_GATEWAY_RES)
        {
            memcpy( &gatewayAddr_g, 
                    p_threadMsg->msgData.sensors.node_addr, 
                    ADDR_LEN);
            print_addr(&gatewayAddr_g);
            valid_gateway_g = true;
            bsp_board_led_off(3);
            segger_printf("Gateway ");
            print_addr(&routerAddr_g);
        }
    }
}

//  88888888888 Y88b   d88P     88888888888 888    888 8888888b.
//      888      Y88b d88P          888     888    888 888   Y88b
//      888       Y88o88P           888     888    888 888    888
//      888        Y888P            888     8888888888 888   d88P
//      888        d888b            888     888    888 8888888P"
//      888       d88888b           888     888    888 888 T88b
//      888      d88P Y88b          888     888    888 888  T88b
//      888     d88P   Y88b         888     888    888 888   T88b

////////////////////////////////////////////////////////////////////////////////
// 
//
void tx_thread (thread_msg_t* p_threadMsg)
{
    segger_println("Thread TX");
    print_pretty_threadMsg(p_threadMsg);
}


//        d8888 8888888b.  8888888b.  8888888b.
//       d88888 888  "Y88b 888  "Y88b 888   Y88b
//      d88P888 888    888 888    888 888    888
//     d88P 888 888    888 888    888 888   d88P
//    d88P  888 888    888 888    888 8888888P"
//   d88P   888 888    888 888    888 888 T88b
//  d8888888888 888  .d88P 888  .d88P 888  T88b
// d88P     888 8888888P"  8888888P"  888   T88b

int compare_addr(uint8_t* p_addr)
{
    for (int i = 0; i < ADDR_LEN; i++)
    {
        if (p_addr[i] != routerAddr_g.addr[i])
        {
            return 0;
        }
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void* set_addr(void)
{
    return memcpy(  routerAddr_g.addr, 
                    (uint8_t*) NRF_FICR->DEVICEADDR, ADDR_LEN);
}

//  .d8888b.   .d88888b.  888b    888 888b    888
// d88P  Y88b d88P" "Y88b 8888b   888 8888b   888
// 888    888 888     888 88888b  888 88888b  888
// 888        888     888 888Y88b 888 888Y88b 888
// 888        888     888 888 Y88b888 888 Y88b888
// 888    888 888     888 888  Y88888 888  Y88888
// Y88b  d88P Y88b. .d88P 888   Y8888 888   Y8888
//  "Y8888P"   "Y88888P"  888    Y888 888    Y888

void reach_gateway (void)
{
    thread_msg_t thread_msg;
    
    memset(&thread_msg, 0, sizeof(thread_msg_t));

    thread_msg.type = R_G_SETUP;
    
    thread_coap_utils_multicast_cmd_request_send(
        thread_ot_instance_get(), &thread_msg);

    // print_pretty_thread_msg(&thread_msg);
}


//  88b      d888        d8888 8888888 888b    888
//  8888b   d8888       d88888   888   8888b   888
//  88888b.d88888      d88P888   888   88888b  888
//  888Y88888P888     d88P 888   888   888Y88b 888
//  888 Y888P 888    d88P  888   888   888 Y88b888
//  888  Y8P  888   d88P   888   888   888  Y88888
//  888   "   888  d8888888888   888   888   Y8888
//  888       888 d88P     888 8888888 888    Y888

#ifndef DOING_TEST
int main(int argc, char * argv[])
{
    init_wdt();
    log_init();

    appHal_rtc_config();

    set_addr();
    segger_printf("My ");
    print_addr(&routerAddr_g);
    valid_gateway_g = true;

    scheduler_init();
    utils_timer_init();
    threadMsgCallback_init(rx_thread);
    thread_instance_init();
    thread_coap_init();
    thread_bsp_init();
    
    update_timer_init();

    for (;;)
    {      
        thread_process();
        app_sched_execute();

        if (valid_gateway_g == false)
        {
            if (appHal_rtc_elapsed_since_ms(base_gateway_search_time_g) > 5000)
                NVIC_SystemReset();
            reach_gateway();
            nrf_delay_ms(500);
        }  
    }
}
#endif