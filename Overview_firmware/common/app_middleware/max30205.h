#ifndef _MAX30205_H_
#define _MAX30205_H_

///////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
///////////////////////////////////////////////////////////////////////////////

#include "nrf_drv_twi.h"
#include "pca10056.h"
// LOG
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "SEGGER_RTT.h"
// HAL
#include "appHal_twi.h"

#include "nrf_delay.h"

///////////////////////////////////////////////////////////////////////////////
//    Definitions
///////////////////////////////////////////////////////////////////////////////

// I2C device address
#define MAX30205_ADDRESS (0x90 >> 1)
// Register Map
#define REG_TEMP   0x00
#define REG_CONF   0x01
#define REG_HYST   0x02
#define REG_OS     0x03
// Register Size (plus register address)
#define REG_CONF_SIZE   1 // bytes
#define REG_OS_SIZE     2 // bytes
#define REG_HYST_SIZE   2 // bytes
#define READ_TEMP_SIZE  2 // bytes
// Default reg data
#define REG_CONF_DATA 0x00

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////

void max30205_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event); 
uint16_t max30205_read_Temp(void);
void max30205_process (uint16_t* temp);

#endif