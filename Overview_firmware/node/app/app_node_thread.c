#include "app_node_thread.h"

static app_thread_t* app_thread;
static tx_msg_buffer_t* buff;
static connection_state_t* conn;

rx_thread_callback_t m_rx_thread_callback;

// 8888888 888b    888 8888888 88888888888
//   888   8888b   888   888       888
//   888   88888b  888   888       888
//   888   888Y88b 888   888       888
//   888   888 Y88b888   888       888
//   888   888  Y88888   888       888
//   888   888   Y8888   888       888
// 8888888 888    Y888 8888888     888

void app_thread_init (rx_thread_callback_t rx_thread_callback, thread_role_t role)
{
    app_thread_init_data_struct();

    ASSERT(rx_thread_callback != NULL);
    rx_callback_init(rx_thread_callback);
    thread_msg_callback_init(app_rx_thread);
    thread_instance_init(role);
    thread_coap_init();
    set_addr();
}

void app_thread_init_data_struct (void)
{
    // app_thread = &app_thread_i;
    app_thread = (app_thread_t*) malloc(sizeof(app_thread_t));
    memset(app_thread, 0, sizeof(app_thread_t));
    buff = &(app_thread->buff);
    conn = &(app_thread->conn);
}

void app_thread_deinit (void)
{
    free(app_thread);
    app_thread = NULL;
    buff       = NULL;
    conn       = NULL;
}

app_thread_t* app_thread_get_data_struct (void)
{
    return (app_thread_t*) app_thread;
}

void rx_callback_init(rx_thread_callback_t rx_thread_callback)
{
    m_rx_thread_callback = rx_thread_callback;
}

void thread_instance_init(thread_role_t role)
{
    thread_configuration_t thread_configuration =
    {
        .role                  = role,
        .autocommissioning     = true,
        .poll_period           = 3000,
        .default_child_timeout = 30,
    };

    thread_init(&thread_configuration);
    thread_cli_init();
    thread_state_changed_callback_set(thread_state_changed_callback);
}

void thread_coap_init(void)
{
    thread_coap_configuration_t thread_coap_configuration =
    {
        .coap_server_enabled               = true,
        .coap_client_enabled               = true,
        .coap_cloud_enabled                = false
    };

    thread_coap_utils_init(&thread_coap_configuration);
}

//  .d8888b. 88888888888     d8888 88888888888 8888888888
// d88P  Y88b    888        d88888     888     888
// Y88b.         888       d88P888     888     888
//  "Y888b.      888      d88P 888     888     8888888
//     "Y88b.    888     d88P  888     888     888
//       "888    888    d88P   888     888     888
// Y88b  d88P    888   d8888888888     888     888
//  "Y8888P"     888  d88P     888     888     8888888888

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
        SEGGER_RTT_printf(0, "OT_CHANGED_THREAD_ROLE changed \n");
    }

    if (flags & OT_CHANGED_THREAD_PARTITION_ID)
    {
        thread_coap_utils_peer_addr_clear();
        SEGGER_RTT_printf(0, "OT_CHANGED_THREAD_PARTITION_ID changed \n");
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
        otIp6SlaacUpdate(thread_ot_instance_get(), conn->m_slaac_addresses,
                         sizeof(conn->m_slaac_addresses) / sizeof(conn->m_slaac_addresses[0]),
                         otIp6CreateRandomIid, NULL);
        SEGGER_RTT_printf(0, "OT_CHANGED_THREAD_NETDATA changed \n");
    }

/*     #ifndef TEST
    SEGGER_RTT_printf(0, "State changed! Flags: 0x%08x \
                Current role: %d\r\n", flags, otThreadGetDeviceRole(p_context));
    #endif */
}

//  8888888b.  Y88b   d88P
//  888   Y88b  Y88b d88P
//  888    888   Y88o88P
//  888   d88P    Y888P
//  8888888P"     d888b
//  888 T88b     d88888b
//  888  T88b   d88P Y88b
//  888   T88b d88P   Y88b

void app_rx_thread(thread_msg_t* threadMsg_p)
{
    m_rx_thread_callback(threadMsg_p);
}

void set_rx_state (thread_role_t role)
{
    if (is_current_role_different(role))
    {
        change_role(role);
    }
}

static inline bool is_current_role_different (thread_role_t role)
{
    return (conn->current_role != role);
}

static inline void change_role (thread_role_t role)
{
    otLinkModeConfig mode;
    memset(&mode, 0, sizeof(mode));
    otInstance* ot_instance = thread_ot_instance_get();
    if (role == RX_OFF_WHEN_IDLE)
    {
        mode.mSecureDataRequests = true;
        mode.mRxOnWhenIdle       = false;
        otLinkSetPollPeriod(ot_instance, 3000);
    }
    else
    {
        mode.mRxOnWhenIdle       = true;
        mode.mSecureDataRequests = true;
        mode.mDeviceType         = true;
        mode.mNetworkData        = true;
    }
    otError error = otThreadSetLinkMode(ot_instance, mode);
    ASSERT(error == OT_ERROR_NONE);
    conn->current_role = role;
}

//  88888888888 Y88b   d88P
//      888      Y88b d88P
//      888       Y88o88P
//      888        Y888P
//      888        d888b
//      888       d88888b
//      888      d88P Y88b
//      888     d88P   Y88b

void tx_thread (thread_msg_t* threadMsg_p)
{
    thread_coap_utils_unicast_cmd_request_send(
                thread_ot_instance_get(), threadMsg_p);

    print_pretty_sensors_thread_msg(threadMsg_p);
}

void thread_send_process (void)
{
    if (is_connected())
    {
        if (!is_buffer_empty())
        {
            if (is_something_to_queue())
            {
                queue_msg();
            }
            else if (is_send_timeout())
            {
                retry_send();
            }
        }
        if (is_msg_sent_with_success())
        {
            select_next_msg();
        }
    }
}

static inline bool is_something_to_queue(void)
{
    return (buff->send_next || buff->retry);
}

static inline void queue_msg(void)
{
    thread_msg_t* thread_msg = &(buff->data[buff->r_ptr]);
    tx_thread(thread_msg);
    buff->timeout = appHal_rtc_elapsed_ms();
    buff->send_next = false;
    buff->retry = false;
}

static inline bool is_send_timeout (void)
{
    return (appHal_rtc_elapsed_since_ms(buff->timeout) > CONN_TIMEOUT);
}

static inline void retry_send (void)
{
    buff->retry = true;
    buff->timeout = appHal_rtc_elapsed_ms();
}

static inline bool is_msg_sent_with_success (void)
{
    return (coap_has_received() && (!buff->send_next));
}

static inline void select_next_msg (void)
{
    buff->r_ptr = (buff->r_ptr + 1) & (THREAD_BUFF_SIZE - 1);
    buff->send_next = true;
    buff->retry = false;
}

// 888888b.   888     888 8888888888 8888888888
// 888  "88b  888     888 888        888
// 888  .88P  888     888 888        888
// 8888888K.  888     888 8888888    8888888
// 888  "Y88b 888     888 888        888
// 888    888 888     888 888        888
// 888   d88P Y88b. .d88P 888        888
// 8888888P"   "Y88888P"  888        888

void app_thread_push_msg(thread_msg_t* thread_msg)
{
    thread_msg_t* ring_buff_dest = &(buff->data[buff->w_ptr]);
    memcpy(ring_buff_dest, thread_msg, sizeof(thread_msg_t));
    buff->w_ptr = (buff->w_ptr + 1) & (THREAD_BUFF_SIZE - 1);
}

static inline bool is_buffer_empty (void)
{
    if (buff->w_ptr == buff->r_ptr)
        return true;
    else
        return false;
}

int buffer_fill (void)
{
    if (buff->w_ptr >= buff->r_ptr)
        return (buff->w_ptr - buff->r_ptr);
    else
        return (THREAD_BUFF_SIZE - (buff->r_ptr - buff->w_ptr));
}

//  .d8888b.   .d88888b.  888b    888 888b    888
// d88P  Y88b d88P" "Y88b 8888b   888 8888b   888
// 888    888 888     888 88888b  888 88888b  888
// 888        888     888 888Y88b 888 888Y88b 888
// 888        888     888 888 Y88b888 888 Y88b888
// 888    888 888     888 888  Y88888 888  Y88888
// Y88b  d88P Y88b. .d88P 888   Y8888 888   Y8888
//  "Y8888P"   "Y88888P"  888    Y888 888    Y888

void check_connection ()
{
    if (is_no_server())
    {
        disconnect();
        if (is_in_mesh_network())
        {
            if (!is_connection_already_started())
            {
                SEGGER_RTT_printf(0, "start_connection \n\r");
                start_connection();
            }
            else
            {
                if(is_connection_timeout())
                {
                    SEGGER_RTT_printf(0, "stop_trying_connect \n\r");
                    stop_trying_connect();
                }
                else
                {
                    SEGGER_RTT_printf(0, ".");
                    let_thread_do_stuff();
                }
            }
        }
    }
    else
    {
        if (!conn->is_connected)
        {
            SEGGER_RTT_printf(0, "\nNow connected \n\r");
        }
        conn->is_connected = true;
    }
}

static inline bool is_no_server (void)
{
    return (!thread_coap_utils_peer_addr_is_set());
}

static inline void disconnect (void)
{
    conn->is_connected = false;
}

bool is_connected(void)
{
    return conn->is_connected;
}

static inline bool is_in_mesh_network (void)
{
    return (otThreadGetDeviceRole(thread_ot_instance_get()) == OT_DEVICE_ROLE_CHILD);
}

static inline bool is_connection_already_started (void)
{
    return (conn->start_connecting == true);
}

static inline void start_connection (void)
{
    conn->start_time = appHal_rtc_elapsed_ms();
    conn->start_connecting = true;
    thread_coap_utils_provisioning_request_send(thread_ot_instance_get());
}

static inline bool is_connection_timeout (void)
{
    return (appHal_rtc_elapsed_since_ms(conn->start_time) > CONN_TIMEOUT);
}

static inline void stop_trying_connect (void)
{
    conn->start_connecting = false;
}

static inline void let_thread_do_stuff (void)
{
    thread_process();
    app_sched_execute();
}

//        d8888 8888888b.  8888888b.  8888888b.
//       d88888 888  "Y88b 888  "Y88b 888   Y88b
//      d88P888 888    888 888    888 888    888
//     d88P 888 888    888 888    888 888   d88P
//    d88P  888 888    888 888    888 8888888P"
//   d88P   888 888    888 888    888 888 T88b
//  d8888888888 888  .d88P 888  .d88P 888  T88b
// d88P     888 8888888P"  8888888P"  888   T88b

bool compare_addr( uint8_t* p_addr)
{
    for (int i = 0; i < ADDR_LEN; i++)
    {
        if (p_addr[i] != conn->node_addr.a[i])
        {
            return false;
        }
    }
    return true;
}

void set_addr(void)
{
    #ifndef TEST
    memcpy(conn->node_addr.a, (uint8_t*) NRF_FICR->DEVICEADDR, ADDR_LEN);
    print_addr(&(conn->node_addr));
    #endif
}

node_addr_t* get_addr(void)
{
    return &conn->node_addr;
}