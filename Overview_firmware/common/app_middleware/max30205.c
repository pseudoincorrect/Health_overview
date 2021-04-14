#include "max30205.h"

///////////////////////////////////////////////////////////////////////////////
//    Static Variables
///////////////////////////////////////////////////////////////////////////////
static nrf_drv_twi_t* g_ptr_twi;
static twi_event_t* g_ptr_twi_event;

///////////////////////////////////////////////////////////////////////////////
//    Static Function Prototypes
///////////////////////////////////////////////////////////////////////////////

static void set_conf (uint8_t mask);
static void set_OS_temp (uint8_t temperature);
static void set_HYST_temp (uint8_t temperature);
static void i2c_read(uint8_t max30205_reg, uint8_t* dataDest, uint8_t dataSize);
static void i2c_write(uint8_t* dataSrc, uint8_t dataSize);
static void wait_xfer(void);

///////////////////////////////////////////////////////////////////////////////
//    Static Functions
///////////////////////////////////////////////////////////////////////////////

static void set_conf (uint8_t mask)
{
    uint8_t data[2] = {REG_CONF, mask};
    i2c_write (data, sizeof(data));
}

static void set_OS_temp (uint8_t temperature)
{
    uint8_t data[3] = {REG_OS, temperature, 0x00};
    int16_t osTemp = temperature << 8;
    i2c_write (data, sizeof(data));
}

static void set_HYST_temp (uint8_t temperature)
{
    uint8_t data[3] = {REG_HYST, temperature, 0x00};
    int16_t osTemp = temperature << 8;
    i2c_write (data, sizeof(data));
}

static void i2c_read(uint8_t max30205_reg, uint8_t* dataDest, uint8_t dataSize)
{
    ret_code_t err_code = nrf_drv_twi_tx(g_ptr_twi, MAX30205_ADDRESS, &max30205_reg, 1, true);
    wait_xfer();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_rx(g_ptr_twi, MAX30205_ADDRESS, dataDest, dataSize); 
    wait_xfer();
    APP_ERROR_CHECK(err_code);
}

static void i2c_write(uint8_t* dataSrc, uint8_t dataSize)
{
    ret_code_t err_code = nrf_drv_twi_tx(g_ptr_twi, MAX30205_ADDRESS, dataSrc, dataSize, false);
    wait_xfer();
    APP_ERROR_CHECK(err_code);
}

static void wait_xfer(void)
{
    while (g_ptr_twi_event->xfer_done == false);
    g_ptr_twi_event->xfer_done = false;
}

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////

void max30205_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event) 
{
    g_ptr_twi = ptr_twi; 
    g_ptr_twi_event = ptr_twi_event;
    // no timeout, normal format, no fault, OS active low, comparator
    set_conf(REG_CONF_DATA); 
    set_OS_temp(48); // 48 degrees Celcius
    set_HYST_temp(45); // 48 degrees Celcius
}

uint16_t max30205_read_Temp()
{
    uint16_t temperature;
    i2c_read(REG_TEMP, (uint8_t*) &temperature, READ_TEMP_SIZE);
    temperature = (temperature >> 8) + (temperature << 8);
    return temperature;
}
           
void max30205_process (uint16_t* temperature) 
{
    *temperature = max30205_read_Temp();
}
