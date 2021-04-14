/////////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
/////////////////////////////////////////////////////////////////////////////////

//-- unity: unit test framework
#include "unity.h"

//-- module being tested
#include "app_thread.h"

//-- Mock dependency
// SDK OpenThread
#include "mock_instance.h"
#include "mock_thread.h"
#include "mock_ip6.h"
#include "mock_link.h"
// Thread abstraction
#include "mock_app_scheduler.h"
#include "mock_thread_coap_utils.h"
#include "mock_thread_utils.h"
// SDK app libraries
#include "mock_appHal_rtc.h"
#include "mock_app_timer.h"
#include "mock_nrf.h"
#include "mock_bsp_thread.h" // need to be integrated to appHal
#include "mock_command_def.h"
#include "mock_SEGGER_RTT.h"
#include "mock_print_utils.h"

/////////////////////////////////////////////////////////////////////////////////
//    DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE TYPES
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE DATA
/////////////////////////////////////////////////////////////////////////////////

int callback_dummy_data;
app_thread_t* app_thread_ptr;

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////

void rx_thread (thread_msg_t* threadMsg_p) 
{;} 

void callback_dummy (void) 
{
    callback_dummy_data = 1;
}

/////////////////////////////////////////////////////////////////////////////////
//    SETUP, TEARDOWN
/////////////////////////////////////////////////////////////////////////////////

void setUp (void) 
{
    callback_dummy_data = 0;
    app_thread_init_data_struct();
    app_thread_ptr = app_thread_get_data_struct();
}

void tearDown (void) {
    app_thread_deinit();
}

/////////////////////////////////////////////////////////////////////////////////
//    TESTS
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// app_thread_init
/////////////////////////////////////////////////////////////////////////////////
void test_app_thread_init(void)
{   
    // rx_callback_init_ExpectAnyArgs();
    threadMsgCallback_init_ExpectAnyArgs();
    thread_init_ExpectAnyArgs();
    thread_cli_init_ExpectAnyArgs();
    thread_state_changed_callback_set_ExpectAnyArgs();
    thread_coap_utils_init_ExpectAnyArgs();

    app_thread_init(rx_thread);
}

/////////////////////////////////////////////////////////////////////////////////
// thread_instance_init
/////////////////////////////////////////////////////////////////////////////////
void test_thread_instance_init(void)
{
    thread_init_ExpectAnyArgs();
    thread_cli_init_ExpectAnyArgs();
    thread_state_changed_callback_set_ExpectAnyArgs();

    thread_instance_init();
}

/////////////////////////////////////////////////////////////////////////////////
// thread_state_changed_callback
/////////////////////////////////////////////////////////////////////////////////
void test_thread_state_changed_callback (void)
{
    uint32_t flag;
    void* void_ptr;

    flag = OT_CHANGED_THREAD_ROLE;
    otThreadGetDeviceRole_ExpectAnyArgsAndReturn(OT_DEVICE_ROLE_DISABLED);
    thread_coap_utils_peer_addr_clear_ExpectAnyArgs();
    thread_state_changed_callback(flag, void_ptr);

    flag = OT_CHANGED_THREAD_PARTITION_ID;
    thread_coap_utils_peer_addr_clear_ExpectAnyArgs();
    thread_state_changed_callback(flag, void_ptr);

    flag = OT_CHANGED_THREAD_NETDATA;
    thread_ot_instance_get_ExpectAnyArgsAndReturn(NULL);
    otIp6SlaacUpdate_ExpectAnyArgs();
    thread_state_changed_callback(flag, void_ptr);
}

/////////////////////////////////////////////////////////////////////////////////
// app_rx_thread
/////////////////////////////////////////////////////////////////////////////////
void test_app_rx_thread (void)
{
    thread_msg_t threadMsg;

    // we expect m_rx_thread_callback NOT to be called
    app_rx_thread(&threadMsg);
    TEST_ASSERT_EQUAL_INT(0, callback_dummy_data);

    // we expect m_rx_thread_callback to be called
    rx_callback_init((rx_thread_callback_t) callback_dummy);
    app_rx_thread(&threadMsg);
    TEST_ASSERT_EQUAL_INT(1, callback_dummy_data);
}

/////////////////////////////////////////////////////////////////////////////////
// THREAD_SEND_PROCESS
/////////////////////////////////////////////////////////////////////////////////
void test_thread_send_process_QUEUE_MSG (void)
{
    // it should Queue a message
    app_thread_ptr->conn.is_connected = true;
    app_thread_ptr->buff.w_ptr = 0;
    app_thread_ptr->buff.r_ptr = 1;
    app_thread_ptr->buff.send_next = true;

    thread_ot_instance_get_ExpectAndReturn(NULL);
    thread_coap_utils_unicast_cmd_request_send_ExpectAnyArgs();
    appHal_rtc_elapsed_ms_ExpectAndReturn(0);
    coap_has_received_ExpectAndReturn(false);

    thread_send_process();
}    

void test_thread_send_process_RETRY_SEND (void)
{
    // it should retry to send a message
    app_thread_ptr->conn.is_connected = true;
    app_thread_ptr->buff.w_ptr = 0;
    app_thread_ptr->buff.r_ptr = 1;

    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(CONN_TIMEOUT + 1);
    appHal_rtc_elapsed_ms_ExpectAndReturn(CONN_TIMEOUT + 2);
    coap_has_received_ExpectAndReturn(false);

    thread_send_process();

    TEST_ASSERT_EQUAL_INT(CONN_TIMEOUT + 2, app_thread_ptr->buff.timeout);
}

void test_thread_send_process_SELECT_NEXT_MSG (void)
{
    // it should retry to acknowledg a message has been sent
    app_thread_ptr->conn.is_connected = true;
    coap_has_received_ExpectAndReturn(true);

    thread_send_process();
}

void test_thread_send_process_NOT_CONNECTED (void)
{
    // it should not do anything
    thread_send_process();
}

/////////////////////////////////////////////////////////////////////////////////
// check_connection
/////////////////////////////////////////////////////////////////////////////////
void test_check_connection_IS_SERVER (void)
{
    // it should set the system as "connected"
    thread_coap_utils_peer_addr_is_set_ExpectAnyArgsAndReturn(true);
    check_connection();
    TEST_ASSERT_TRUE(app_thread_ptr->conn.is_connected);
}

void test_check_connection_START_CONNECTION (void)
{
    // it should set the start connection flag
    app_thread_ptr->conn.current_role = RX_OFF_WHEN_IDLE;

    thread_coap_utils_peer_addr_is_set_ExpectAnyArgsAndReturn(false);
    thread_ot_instance_get_ExpectAndReturn(NULL);
    otThreadGetDeviceRole_ExpectAnyArgsAndReturn(OT_DEVICE_ROLE_CHILD);
    appHal_rtc_elapsed_ms_ExpectAndReturn(1);
    thread_ot_instance_get_ExpectAndReturn(NULL);
    thread_coap_utils_provisioning_request_send_ExpectAnyArgs();

    check_connection();

    TEST_ASSERT_TRUE(app_thread_ptr->conn.start_connecting);
}

void test_check_connection_STOP_TRYING (void)
{
    // it should stop trying to connect
    app_thread_ptr->conn.current_role = RX_OFF_WHEN_IDLE;
    app_thread_ptr->conn.start_connecting = true;

    thread_coap_utils_peer_addr_is_set_ExpectAnyArgsAndReturn(false);
    thread_ot_instance_get_ExpectAndReturn(NULL);
    otThreadGetDeviceRole_ExpectAnyArgsAndReturn(OT_DEVICE_ROLE_CHILD);
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(CONN_TIMEOUT + 1);

    check_connection();

    TEST_ASSERT_FALSE(app_thread_ptr->conn.start_connecting);
}

void test_check_connection_LET_THREAD (void)
{
    // it should let thread scheduler do his tasks
    app_thread_ptr->conn.current_role = RX_OFF_WHEN_IDLE;
    app_thread_ptr->conn.start_connecting = true;

    thread_coap_utils_peer_addr_is_set_ExpectAnyArgsAndReturn(false);
    thread_ot_instance_get_ExpectAndReturn(NULL);
    otThreadGetDeviceRole_ExpectAnyArgsAndReturn(OT_DEVICE_ROLE_CHILD);
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(CONN_TIMEOUT - 1);
    thread_process_ExpectAnyArgs();
    app_sched_execute_ExpectAnyArgs();
    check_connection();

    TEST_ASSERT_TRUE(app_thread_ptr->conn.start_connecting);
}

/////////////////////////////////////////////////////////////////////////////////
// COMPARE_ADDR
/////////////////////////////////////////////////////////////////////////////////
void test_compare_addr (void)
{
    bool ret;
    uint8_t test_arr_valid[6] = {1,2,3,4,5,6};
    uint8_t test_arr_not_valid[6] = {1,2,3,4,5,7};
    memcpy(app_thread_ptr->conn.node_addr.addr, test_arr_valid, ADDR_LEN);

    ret = compare_addr(test_arr_valid);
    TEST_ASSERT_TRUE(ret);

    ret = compare_addr(test_arr_not_valid);
    TEST_ASSERT_FALSE(ret);
}