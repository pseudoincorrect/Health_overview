#include "app_sensors.h"

const nrf_drv_timer_t timesensors = NRF_DRV_TIMER_INSTANCE(2);
app_sensors_t* app_sensors;
app_sensors_t app_sensors_i;

#if SENSORS_MOCK
    n_sensors_t n_sensors_mock;
#endif

// 8888888 888b    888 8888888 88888888888
//   888   8888b   888   888       888
//   888   88888b  888   888       888
//   888   888Y88b 888   888       888
//   888   888 Y88b888   888       888
//   888   888  Y88888   888       888
//   888   888   Y8888   888       888
// 8888888 888    Y888 8888888     888

void app_sensors_init(void)
{
    app_sensor_init_variables();
    sensors_timer_init();
    #if (!SENSORS_MOCK)
        appHal_twi_init();
        SEGGER_RTT_printf(0, " appHal_twi_init done\n\r");
        max30205_init(appHal_twi_get_instance(), appHal_twi_get_event_instance());
        SEGGER_RTT_printf(0, " max30205_init done\n\r");
        heart_rate_init(appHal_twi_get_instance(), appHal_twi_get_event_instance());
        MAX30105_shutDown();
    #else
        init_mock_node_sensors(&n_sensors_mock);
    #endif

}

void app_sensor_init_variables(void)
{
    // app_sensors = malloc(sizeof(app_sensors_t));
    // memset(app_sensors, 0, sizeof(app_sensors_t));
    app_sensors = &app_sensors_i;
}

void app_sensor_deinit_variables(void)
{
    // free(app_sensors);
}

void sensors_timer_init(void)
{
    uint32_t time_ms = 10000;
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&timesensors, &timer_cfg, sensors_timer_handler);
    APP_ERROR_CHECK(err_code);
    time_ticks = nrf_drv_timer_ms_to_ticks(&timesensors, time_ms);
    nrf_drv_timer_extended_compare(
        &timesensors, NRF_TIMER_CC_CHANNEL0, time_ticks,
        NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&timesensors);
}

//  888    888        d8888 888b    888 8888888b.  888      8888888888
//  888    888       d88888 8888b   888 888  "Y88b 888      888
//  888    888      d88P888 88888b  888 888    888 888      888
//  8888888888     d88P 888 888Y88b 888 888    888 888      8888888
//  888    888    d88P  888 888 Y88b888 888    888 888      888
//  888    888   d88P   888 888  Y88888 888    888 888      888
//  888    888  d8888888888 888   Y8888 888  .d88P 888      888
//  888    888 d88P     888 888    Y888 8888888P"  88888888 8888888888

static void sensors_timer_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            #if(!MOCK_SENSORS)
                app_sensors->update_sensors_init = true;
            #else
                mock_process();
            #endif
            break;
        default:
            break;
    }
}

// 8888888b.  8888888b.   .d88888b.   .d8888b.
// 888   Y88b 888   Y88b d88P" "Y88b d88P  Y88b
// 888    888 888    888 888     888 888    888
// 888   d88P 888   d88P 888     888 888
// 8888888P"  8888888P"  888     888 888
// 888        888 T88b   888     888 888    888
// 888        888  T88b  Y88b. .d88P Y88b  d88P
// 888        888   T88b  "Y88888P"   "Y8888P"
 
void mock_process (void)
{
    #if SENSORS_MOCK
        update_mock_node_sensors(&n_sensors_mock);
        app_sensors->sensors_updated = true;
    #endif
}

void sensors_process (n_sensors_t* sensors)
{
    // if (can_start_acquisition())
    // {
    //     start_acquisition();
    // }
    // let_heart_rate_sensor_work();

    #if(SENSORS_MOCK)
        copy_mock_node_sensors(sensors, &n_sensors_mock);
    #else
        if (can_start_acquisition())
        {
            start_acquisition();
        }

        if (can_update_acquisition())
        {
            if (is_acquisition_timeout())
            {
                acquisition_timeout();
                return;
            }

            let_heart_rate_sensor_work();

            if (is_heart_rate_updated())
            {
                register_heart_rate();
            }

            if (is_sample_buffer_full())
            {
                update_sensors_states(sensors);
            }
        }
    #endif
}

static inline bool can_start_acquisition (void)
{
    return (app_sensors->update_sensors_init);
}

static inline void start_acquisition (void)
{
    app_sensors->sample_count = 0;
    app_sensors->update_sensors_init = false;
    app_sensors->update_sensors = true;
    app_sensors->base_time = appHal_rtc_elapsed_ms();
    HR_wake_up();
}

static inline bool can_update_acquisition (void)
{
    return (app_sensors->update_sensors);
}

static inline bool is_acquisition_timeout (void)
{
    return (appHal_rtc_elapsed_since_ms(app_sensors->base_time) > SENSORS_TIMEOUT);
}

static inline void acquisition_timeout (void)
{
    app_sensors->update_sensors = false;
    MAX30105_shutDown();
}

static inline void let_heart_rate_sensor_work (void)
{
    uint8_t heart_rate;
    bool heart_rate_updated;
    heart_rate_updated = heart_rate_process(&heart_rate);

    if (heart_rate_updated)
    {
        app_sensors->heart_rate_updated = true;
        app_sensors->current_heart_rate = heart_rate;
    }
}

static inline bool is_heart_rate_updated (void)
{
    return (app_sensors->heart_rate_updated);
}

static inline void register_heart_rate (void)
{
    uint16_t index = app_sensors->sample_count;
    app_sensors->heart_rate_arr[index] = app_sensors->current_heart_rate ;
    app_sensors->sample_count++;
    app_sensors->heart_rate_updated = false;
}

static inline bool is_sample_buffer_full (void)
{
    return (app_sensors->sample_count >= SAMPLE_SIZE);
}

static inline void update_sensors_states (n_sensors_t* sensors)
{
    sensors->temperature = max30205_read_Temp();
    sensors->heart_rate   = array_mean_uint8(app_sensors->heart_rate_arr, SAMPLE_SIZE);
    sensors->oxymetry    = 0;
    sensors->humidity    = 0;
    sensors->dehydration = 0;
    sensors->reserved1   = app_sensors->id_packet_cnt;
    sensors->reserved2   = 0;

    app_sensors->update_sensors = false;
    app_sensors->id_packet_cnt++;
    app_sensors->sensors_updated = true;

    MAX30105_shutDown();
    // SEGGER_RTT_printf(0, "heart rate %d \n\r", sensors->heart_rate);
}

uint8_t array_mean_uint8 (uint8_t* array, uint8_t size)
{
    uint32_t sum = 0;
    for (int i=0; i<size; i++)
        sum += array[i];
    sum /= size;
    return (uint8_t) sum;
}

//  .d8888b.  8888888888 88888888888 88888888888 8888888888 8888888b.
// d88P  Y88b 888            888         888     888        888   Y88b
// 888    888 888            888         888     888        888    888
// 888        8888888        888         888     8888888    888   d88P
// 888  88888 888            888         888     888        8888888P"
// 888    888 888            888         888     888        888 T88b
// Y88b  d88P 888            888         888     888        888  T88b
//  "Y8888P88 8888888888     888         888     8888888888 888   T88b

bool is_new_sensors_data(void)
{
    if (app_sensors->sensors_updated)
    {
        app_sensors->sensors_updated = false;
        return true;
    }
    return false;
}

bool is_sensor_updating(void){
    return (app_sensors->update_sensors_init | app_sensors->update_sensors);
}

app_sensors_t* get_app_sensors(void)
{
    return app_sensors;
}
