#include "app_gateway_thread.h"

gen_fifo_t* rx_fifo_m;

//  888    888        d8888 888b    888 8888888b.  888      8888888888 8888888b.  
//  888    888       d88888 8888b   888 888  "Y88b 888      888        888   Y88b 
//  888    888      d88P888 88888b  888 888    888 888      888        888    888 
//  8888888888     d88P 888 888Y88b 888 888    888 888      8888888    888   d88P 
//  888    888    d88P  888 888 Y88b888 888    888 888      888        8888888P"  
//  888    888   d88P   888 888  Y88888 888    888 888      888        888 T88b   
//  888    888  d8888888888 888   Y8888 888  .d88P 888      888        888  T88b  
//  888    888 d88P     888 888    Y888 8888888P"  88888888 8888888888 888   T88b 

//  State changed callback
void thread_state_changed_callback(uint32_t flags, void * p_context)
{
    if (flags & OT_CHANGED_THREAD_ROLE)
    {
        switch (otThreadGetDeviceRole(p_context))
        {
            case OT_DEVICE_ROLE_CHILD:
            case OT_DEVICE_ROLE_ROUTER:
            case OT_DEVICE_ROLE_LEADER:
                thread_coap_utils_provisioning_enable(true);
                break;

            case OT_DEVICE_ROLE_DISABLED:
            case OT_DEVICE_ROLE_DETACHED:
            default:
                thread_coap_utils_provisioning_enable(false);
                break;
        }
    }
    SEGGER_RTT_printf(0, "State changed! Flags: 0x%08x Current role: %d\r\n",
                 flags,
                 otThreadGetDeviceRole(p_context));
}

//  8888888 888b    888 8888888 88888888888 
//    888   8888b   888   888       888     
//    888   88888b  888   888       888     
//    888   888Y88b 888   888       888     
//    888   888 Y88b888   888       888     
//    888   888  Y88888   888       888     
//    888   888   Y8888   888       888     
//  8888888 888    Y888 8888888     888  

void app_thread_gateway_init (gen_fifo_t* rx_fifo)
{
    rx_fifo_m = rx_fifo;
    thread_msg_callback_init(rx_thread);
    thread_coap_utils_provisioning_timer_init();
    thread_instance_init();
    thread_coap_init();
    enable_provisionning(true);
}

// Function for initializing the Thread Stack.
void thread_instance_init(void)
{
    thread_configuration_t thread_configuration =
    {
        .role                  = RX_ON_WHEN_IDLE,
        .autocommissioning     = true,
        .poll_period           = 2500,
        .default_child_timeout = 10,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);
}

// Function for initializing the Constrained Application Protocol Module.
void thread_coap_init(void)
{
    thread_coap_configuration_t thread_coap_configuration =
    {
        .coap_server_enabled               = true,
        .coap_client_enabled               = true,
        .coap_cloud_enabled                = false,
    };

    thread_coap_utils_init(&thread_coap_configuration);
}

// 8888888b.   .d88888b.  888     888 88888888888 8888888888 8888888b.  
// 888   Y88b d88P" "Y88b 888     888     888     888        888   Y88b 
// 888    888 888     888 888     888     888     888        888    888 
// 888   d88P 888     888 888     888     888     8888888    888   d88P 
// 8888888P"  888     888 888     888     888     888        8888888P"  
// 888 T88b   888     888 888     888     888     888        888 T88b   
// 888  T88b  Y88b. .d88P Y88b. .d88P     888     888        888  T88b  
// 888   T88b  "Y88888P"   "Y88888P"      888     8888888888 888   T88b 

void reach_gateway_res (void)
{
    thread_msg_t thread_msg;

    memset(&thread_msg, 0, sizeof(thread_msg_t));
    
    thread_msg.type = G_R_SETUP;

    // print_pretty_thread_msg(&thread_msg);
    
    thread_coap_utils_multicast_cmd_request_send(
        thread_ot_instance_get(), &thread_msg);

}

//  8888888b.  Y88b   d88P
//  888   Y88b  Y88b d88P 
//  888    888   Y88o88P  
//  888   d88P    Y888P   
//  8888888P"     d888b   
//  888 T88b     d88888b  
//  888  T88b   d88P Y88b 
//  888   T88b d88P   Y88b

void rx_thread(thread_msg_t* thread_msg) {
    uint8_t ret;
    fifo_data_packet_t fifo_data_packet;

    // SEGGER_RTT_printf(0, "\n");
    // print_pretty_sensors_thread_msg(thread_msg);
    // SEGGER_RTT_printf(0, "\n");

    if (thread_msg->type == R_G_SETUP)
    {
        reach_gateway_res();
    }
    else if (thread_msg->type == N_G_SENSORS || thread_msg->type == N_G_NOTIF)
    {
        SEGGER_RTT_printf(0, "msg_type = %d \n\r", thread_msg->type);
        
        memcpy( &fifo_data_packet, thread_msg, sizeof(thread_msg_t) );
        //    print_pretty_thread_msg((thread_msg_t*) &fifo_data_packet);  
        ret = gen_fifo_write(rx_fifo_m, &fifo_data_packet);
        if (ret != NRF_SUCCESS) 
        {
            NRF_LOG_WARNING("write failed, flushing fifo");
            gen_fifo_flush(rx_fifo_m);
        }
    }
}

// 88888888888 Y88b   d88P 
//     888      Y88b d88P  
//     888       Y88o88P   
//     888        Y888P    
//     888        d888b    
//     888       d88888b   
//     888      d88P Y88b  
//     888     d88P   Y88b 
void send_notif (thread_msg_t* thread_msg) 
{
    print_pretty_nofif_thread_msg(thread_msg);
    thread_coap_utils_multicast_cmd_request_send(thread_ot_instance_get(), thread_msg);
}

void create_fake_thead_msg (thread_msg_t* thread_msg)
{
    thread_msg->type                    = G_N_NOTIF;
    thread_msg->content.notif.addr.a[0] = 0x50;
    thread_msg->content.notif.addr.a[1] = 0xEB;
    thread_msg->content.notif.addr.a[2] = 0x1E;
    thread_msg->content.notif.addr.a[3] = 0x1C;
    thread_msg->content.notif.addr.a[4] = 0xE7;
    thread_msg->content.notif.addr.a[5] = 0x8A;
    thread_msg->content.notif.data      = NODE_SETUP;
} 

void send_fake_notif(void)
{
    thread_msg_t thread_msg;
    create_fake_thead_msg(&thread_msg);
    print_pretty_nofif_thread_msg(&thread_msg);
    thread_coap_utils_multicast_cmd_request_send(thread_ot_instance_get(), &thread_msg);
}

// 888    888 8888888888 888      8888888b.  8888888888 8888888b.  
// 888    888 888        888      888   Y88b 888        888   Y88b 
// 888    888 888        888      888    888 888        888    888 
// 8888888888 8888888    888      888   d88P 8888888    888   d88P 
// 888    888 888        888      8888888P"  888        8888888P"  
// 888    888 888        888      888        888        888 T88b   
// 888    888 888        888      888        888        888  T88b  
// 888    888 8888888888 88888888 888        8888888888 888   T88b 

bool compare_addr (uint8_t* addr_1, uint8_t* addr_2)
{
    for(size_t i = 0; i < ADDR_LEN; i++)
    {
        if (addr_1[i] != addr_2[i])
            return false;
    }
    return true;
}

void set_msg_addr (node_addr_t* addr, node_addr_t* msg_addr)
{
    memcpy(msg_addr, addr, sizeof(node_addr_t));
}
