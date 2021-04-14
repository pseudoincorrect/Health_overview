#ifndef _APP_SENSORS_H
#define _APP_SENSORS_H

///////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include "nrf_drv_timer.h"
#include "command_def.h"
#include "max30205.h"
#include "heartRate.h"
#include "appHal_twi.h"
#include "node_mock.h"

#define SENSORS_TIMEOUT 5000

typedef struct {
    bool update_sensors_init;
    bool update_sensors;
    bool heart_rate_updated;
    bool sensors_updated;
    uint8_t current_heart_rate;
    uint8_t heart_rate_arr[SAMPLE_SIZE];
    uint8_t sample_count;
    uint16_t max20205Temp;
    uint16_t id_packet_cnt;
    uint32_t base_time;
} app_sensors_t;

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////
void mock_process (void);
static inline void acquisition_timeout(void);
void app_sensors_init(void);
uint8_t array_mean_uint8(uint8_t* array, uint8_t size);
static inline bool can_start_acquisition(void);
static inline bool can_update_acquisition(void);
app_sensors_t* get_app_sensors(void);
void app_sensor_init_variables(void);
void app_sensor_deinit_variables(void);
static inline bool is_acquisition_timeout(void);
static inline bool is_heart_rate_updated(void);
bool is_new_sensors_data(void);
static inline bool is_sample_buffer_full(void);
bool is_sensor_updating(void);
static inline void register_heart_rate(void);
void sensors_process (n_sensors_t* sensors);
static void sensors_timer_handler(nrf_timer_event_t event_type, void* p_context);
void sensors_timer_init(void);
static inline void start_acquisition(void);
static inline void update_sensors_states (n_sensors_t* sensors);
static inline void let_heart_rate_sensor_work (void);

#endif