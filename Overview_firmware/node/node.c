////////////////////////////////////////////////////////////////////////////////
//    Included Files
////////////////////////////////////////////////////////////////////////////////

#include "node.h"

////////////////////////////////////////////////////////////////////////////////
//    Define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//    Global Variables
////////////////////////////////////////////////////////////////////////////////

n_sensors_t sensors;
power_mode_t power_mode;

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

void bsp_event_handler(uint32_t timePushed)
{
    if (timePushed > 30000)
    {
        set_high_power_mode();
    }
    else
    {
        send_notification(N_G_NOTIF);
    }
    notification_from_base();
}

void rx_thread(thread_msg_t* thread_msg)
{
    print_pretty_nofif_thread_msg(thread_msg);

    if (thread_msg->type != G_N_NOTIF) { return; }
    node_addr_t* addr_msg = (node_addr_t*) &(thread_msg->content.notif.addr);
    if (compare_addr((uint8_t*) addr_msg))
    {
        notification_from_base();
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

void print_banner(void)
{
    SEGGER_RTT_printf(0, "\n");
    SEGGER_RTT_printf(0, "************************************************************** \n");
    SEGGER_RTT_printf(0, "*                                                            * \n");
    SEGGER_RTT_printf(0, "*                      NODE STARTED !                        * \n");
    SEGGER_RTT_printf(0, "*                                                            * \n");
    SEGGER_RTT_printf(0, "************************************************************** \n");
}

void init (void)
{
    init_power_mode();
    init_wdt();
    node_bsp_init();
    log_init();
    print_banner();
    appHal_rtc_config();
    scheduler_init();
    utils_timer_init();
    app_thread_init (rx_thread, RX_OFF_WHEN_IDLE);
    app_sensors_init();
    print_banner();
}

void init_power_mode(void)
{
    set_low_power_mode();
}

void utils_timer_init(void)
{
    uint32_t error_code = app_timer_init();
    APP_ERROR_CHECK(error_code);
}

void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

void node_bsp_init(void)
{
    buttons_init(bsp_event_handler);
    gpio_output_init();
    notification_from_base();
}

// 888b    888  .d88888b. 88888888888 8888888 8888888888 
// 8888b   888 d88P" "Y88b    888       888   888        
// 88888b  888 888     888    888       888   888        
// 888Y88b 888 888     888    888       888   8888888    
// 888 Y88b888 888     888    888       888   888        
// 888  Y88888 888     888    888       888   888        
// 888   Y8888 Y88b. .d88P    888       888   888        
// 888    Y888  "Y88888P"     888     8888888 888        

void send_notification(uint8_t type)
{
    thread_msg_t msg;
    msg.type = N_G_NOTIF;
    node_addr_t* addr_src = get_addr();

    msg.content.notif.data = N_G_NOTIF;
    node_addr_t* addr_dest = &msg.content.notif.addr;

    memcpy(addr_dest, addr_src, sizeof(node_addr_t));
    app_thread_push_msg(&msg);
}

// 8888888b.  888     888  .d8888b.  888    888 
// 888   Y88b 888     888 d88P  Y88b 888    888 
// 888    888 888     888 Y88b.      888    888 
// 888   d88P 888     888  "Y888b.   8888888888 
// 8888888P"  888     888     "Y88b. 888    888 
// 888        888     888       "888 888    888 
// 888        Y88b. .d88P Y88b  d88P 888    888 
// 888         "Y88888P"   "Y8888P"  888    888 

void push_sensors_msg (void)
{
    thread_msg_t msg;
    msg.type = N_G_SENSORS;
    
    node_addr_t* addr_dest = &msg.content.sensors.addr;
    n_sensors_t* sensors_dest = &msg.content.sensors.origin.node;
    node_addr_t* addr_src = get_addr();

    memcpy(sensors_dest, &sensors, sizeof(n_sensors_t));
    memcpy(addr_dest, addr_src, sizeof(node_addr_t));

    app_thread_push_msg(&msg);
    
    // print_pretty_sensors_thread_msg(&msg);
}

// 8888888b.   .d88888b.  888       888 8888888888 8888888b.  
// 888   Y88b d88P" "Y88b 888   o   888 888        888   Y88b 
// 888    888 888     888 888  d8b  888 888        888    888 
// 888   d88P 888     888 888 d888b 888 8888888    888   d88P 
// 8888888P"  888     888 888d88888b888 888        8888888P"  
// 888        888     888 88888P Y88888 888        888 T88b   
// 888        Y88b. .d88P 8888P   Y8888 888        888  T88b  
// 888         "Y88888P"  888P     Y888 8888888888 888   T88b 

void main_power_mode_decision (void)
{
    if (is_low_power_mode())
    {
        if (can_goto_sleep())
        { 
            set_rx_state(RX_OFF_WHEN_IDLE);
            thread_sleep();
        }
        else
        {
            if (can_transmit_something())
            {
                set_rx_state(RX_ON_WHEN_IDLE);
            }
            else
            {
                set_rx_state(RX_OFF_WHEN_IDLE);
            }
        }
    }
    else
    {
        if (is_time_for_low_power_mode())
        {
            set_low_power_mode();
        }
    }
}

void set_high_power_mode(void)
{
    set_rx_state(RX_ON_WHEN_IDLE);
    power_mode.mode = HIGH_POWER;
    power_mode.time_activated = appHal_rtc_elapsed_ms();
    SEGGER_RTT_printf(0, "entering high power mode \n\r");
}

void set_low_power_mode(void)
{
    set_rx_state(RX_OFF_WHEN_IDLE);
    power_mode.mode = LOW_POWER;
    power_mode.time_activated = 0;
    SEGGER_RTT_printf(0, "exiting high power mode \n\r");
}

bool is_time_for_low_power_mode (void)
{
    uint32_t activated_since = appHal_rtc_elapsed_since_ms(power_mode.time_activated);
    return (activated_since >= HIGH_POWER_MODE_DURATION);
}

bool is_low_power_mode (void)
{
    return (power_mode.mode == LOW_POWER);
}

static inline bool can_goto_sleep (void)
{
    // sensors not updating AND (not connected OR buffer empty)
    return (
        !is_sensor_updating() 
        && 
        (!is_connected() || (buffer_fill() <=  TX_BUFF_THRESHOLD) )
    );
}

static inline bool can_transmit_something (void)
{
    // if connect AND has enough data to be sent
    return (is_connected() && (buffer_fill() > 5));
}

//  88b      d888        d8888 8888888 888b    888
//  8888b   d8888       d88888   888   8888b   888
//  88888b.d88888      d88P888   888   88888b  888
//  888Y88888P888     d88P 888   888   888Y88b 888
//  888 Y888P 888    d88P  888   888   888 Y88b888
//  888  Y8P  888   d88P   888   888   888  Y88888
//  888   "   888  d8888888888   888   888   Y8888
//  888       888 d88P     888 8888888 888    Y888

void main_unblocking_process (void)
{
    sensors_process(&sensors);
    if (is_new_sensors_data())
    {
        push_sensors_msg();
    }
    thread_send_process();
    check_connection();
    feed_the_dog();
    thread_process();
    app_sched_execute();
}

int main (int argc, char * argv[])
{
    init();

    for (;;)
    {   
        // thread_process();
        // app_sched_execute();   
        // feed_the_dog();

        main_unblocking_process();
        main_power_mode_decision();
    }
}