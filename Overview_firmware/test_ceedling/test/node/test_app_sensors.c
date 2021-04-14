/////////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
/////////////////////////////////////////////////////////////////////////////////

//-- unity: unit test framework
#include "unity.h"

//-- module being tested
#include "app_sensors.h"

//-- Mock dependency
#include "mock_nrf_drv_timer.h"
#include "mock_max30205.h"
#include "mock_max30105.h"
#include "mock_heartRate.h"
#include "mock_appHal_twi.h"
#include "mock_appHal_rtc.h"
#include "mock_app_error.h"
#include "mock_nrfx_timer.h"

/////////////////////////////////////////////////////////////////////////////////
//    DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE TYPES
/////////////////////////////////////////////////////////////////////////////////
app_sensors_t* test_app_sensors;
sensors_t sensors ;

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE DATA
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//    PRIVATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//    SETUP, TEARDOWN
/////////////////////////////////////////////////////////////////////////////////

void setUp(void)
{
    app_sensor_init_variables();
    test_app_sensors = get_app_sensors();
}

void tearDown(void)
{
    app_sensor_deinit_variables();
}

/////////////////////////////////////////////////////////////////////////////////
//    TESTS
/////////////////////////////////////////////////////////////////////////////////

void test_sensors_process_START_UPDATE(void)
{
    // it should start the acquision (enable the update_sensors flag)
    test_app_sensors->update_sensors_init = true;
    appHal_rtc_elapsed_ms_ExpectAndReturn(SENSORS_TIMEOUT - 1000);
    HR_wake_up_ExpectAnyArgsAndReturn(true);
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(SENSORS_TIMEOUT - 1);
    heart_rate_process_ExpectAnyArgsAndReturn(false);

    sensors_process(&sensors);

    TEST_ASSERT_TRUE(test_app_sensors->update_sensors);
}

void test_sensors_process_TIMEOUT(void)
{
    // it should trigger the timeout and stop updating sensors
    test_app_sensors->update_sensors = true;
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(SENSORS_TIMEOUT + 1);
    MAX30105_shutDown_ExpectAnyArgs();

    sensors_process(&sensors);

    TEST_ASSERT_FALSE(test_app_sensors->update_sensors);
}

void test_sensors_process_REGISTER_H_R(void)
{
    // it should register heart and increase sample count
    test_app_sensors->update_sensors = true;
    test_app_sensors->sample_count = 1;
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(SENSORS_TIMEOUT - 1);
    heart_rate_process_ExpectAnyArgsAndReturn(true);

    sensors_process(&sensors);

    TEST_ASSERT_EQUAL_INT(2, test_app_sensors->sample_count);
}

void test_sensors_process_UPDATE_SENSORS_STATES(void)
{
    // it should update sensorState and call update_sensors_states
    test_app_sensors->update_sensors = true;
    test_app_sensors->sample_count = SAMPLE_SIZE-1;
    appHal_rtc_elapsed_since_ms_ExpectAnyArgsAndReturn(SENSORS_TIMEOUT - 1);
    heart_rate_process_ExpectAnyArgsAndReturn(true);

    max30205_read_Temp_ExpectAndReturn(1);
    MAX30105_shutDown_ExpectAnyArgs();

    sensors_process(&sensors);
}