#ifndef MYHAL_SAADC_H
#define MYHAL_SAADC_H

#include <stdint.h>
#include "sdk_errors.h"
#include "nrf_mesh_sdk.h"

#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"

#include "node_board.h"

#define SAMPLES_IN_BUFFER      (1)

void appHal_saadc_sampling_event_enable(void);

void appHal_saadc_init(void* saadc_callback);

void appHal_saadc_init_simple(void);

void appHal_saadc_sample_simple(nrf_saadc_input_t saadc_pin, int16_t* result_ptr);

void appHal_saadc_sampling_event_init(void);

void timer_handler(nrf_timer_event_t event_type, void * p_context);


#endif
