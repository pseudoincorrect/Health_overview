#include "nodeHal_oneWire.h"

#define US_TO_TICK(X) (X*16)
void
DHT22_Init (void)
{
    // the alim for the DHT come from one of the GPIO pin of the MCU (no choice)
    nodeHal_init( PIN_DHT_VDD_MASK, OUTPUT_DIR);
    nrf_gpio_pin_write(PIN_DHT_VDD, STD_ON);

    nrf_timer_mode_set(DHT22_TIMER, NRF_TIMER_MODE_TIMER);
    nrf_timer_frequency_set(DHT22_TIMER, NRF_TIMER_FREQ_16MHz);
    nrf_timer_bit_width_set(DHT22_TIMER, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_START);

    nrf_gpio_cfg_output (31); // DEBUGPIN31
    nrf_gpio_pin_write(31, 0); // DEBUGPIN31

    nrf_gpio_pin_write(31, 1); // DEBUGPIN31
    nrf_gpio_pin_write(31, 0); // DEBUGPIN31
}

static uint32_t read_timer (void)
{
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CAPTURE0);
    return nrf_timer_cc_read(DHT22_TIMER, NRF_TIMER_CC_CHANNEL0);
}

//
static uint8_t DHT22_GetReadings (dht22_data *out)
// static uint8_t DHT22_GetReadings (void)
{
    uint8_t i;
    uint16_t cnt;
    uint32_t timer_val;

    // Switch pin to output
    nrf_gpio_cfg_output (PIN_DHT_DATA);
    nrf_gpio_pin_write(PIN_DHT_DATA, STD_OFF);

    // Generate start impulse
    nrf_gpio_pin_write(PIN_DHT_DATA, STD_ON);
    nrf_gpio_pin_write(PIN_DHT_DATA, STD_OFF);
    nrf_delay_ms(2);
    nrf_gpio_pin_write(PIN_DHT_DATA, STD_ON);

    // Switch pin to input without Pull-Up
    nrf_gpio_cfg_input (PIN_DHT_DATA, NRF_GPIO_PIN_PULLUP);


    // Wait for AM2302 to begin communication (20-40us)
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
    // Wait DHT data pin High or timeout 1600 ticks at 16 MHz = 100 us
    while (((cnt = read_timer()) < US_TO_TICK(100)) && nrf_gpio_pin_read(PIN_DHT_DATA))
    { ; }
    if (cnt >= US_TO_TICK(100))
    {
        SEGGER_RTT_printf(0, "DHT22: DHT22_RCV_NO_RESPONSE cnt = %d\n\r", cnt);
        return DHT22_RCV_NO_RESPONSE;
    }



    // Check ACK strobe from sensor (goes to LOW)
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
    // Wait DHT data pin Low or timeout
    while (((cnt = read_timer()) < US_TO_TICK(100)) && (!nrf_gpio_pin_read(PIN_DHT_DATA)))
    { ; }
    if ((cnt < US_TO_TICK(65)) || (cnt > US_TO_TICK(95)))
    {
        SEGGER_RTT_printf(0, "DHT22: DHT22_RCV_BAD_ACK1 cnt = %d\n\r", cnt);
        return DHT22_RCV_BAD_ACK1;
    }



    // Check ACK strobe from sensor (goes to HIGH)
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
    // Wait DHT data pin High or timeout
    while (((cnt = read_timer()) < US_TO_TICK(100)) && nrf_gpio_pin_read(PIN_DHT_DATA))
    { ; }
    if ((cnt < US_TO_TICK(65)) || (cnt > US_TO_TICK(95)))
    {
        SEGGER_RTT_printf(0, "DHT22: DHT22_RCV_BAD_ACK2 cnt = %d\n\r", cnt);
        return DHT22_RCV_BAD_ACK2;
    }


    // ACK strobe received --> receive 40 bits
    i = 0;
    nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);

    while (i < 40)
    {
        // Wait DHT data pin Low or timeout
        while (((cnt = read_timer()) < US_TO_TICK(100)) && (!nrf_gpio_pin_read(PIN_DHT_DATA)) )
        { ; }
        if (cnt > US_TO_TICK(80))
        {
            // invalid bit start impulse length (LOW part)
            SEGGER_RTT_printf(0, "DHT22: Invalid time LOW %d\n\r", cnt);
            out->bits[i] = 0xff;
            // Wait DHT data pin High or timeout
            while (((cnt = read_timer()) < US_TO_TICK(100)) && nrf_gpio_pin_read(PIN_DHT_DATA))
            { ; }
            nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
        }
        else
        {
            // High part containing the data 0 if short pulse, 1 otherwise
            // Measure bit impulse length (T_h0 = 25us, T_h1 = 70us)
            nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
            // Wait DHT data pin High or timeout
            while (((cnt = read_timer()) < US_TO_TICK(100)) && nrf_gpio_pin_read(PIN_DHT_DATA))
            { ; }
            nrf_timer_task_trigger(DHT22_TIMER, NRF_TIMER_TASK_CLEAR);
            out->bits[i] = (cnt < US_TO_TICK(100)) ? (uint8_t) (cnt/16) : 0xff;
        }
        i++;
    }


    for (i = 0; i < 40; i++)
        if (out->bits[i] == 0xff)
        {
            SEGGER_RTT_printf(0, "DHT22: DHT22_RCV_TIMEOUT bit[i], i = %d\n\r", i);
            return DHT22_RCV_TIMEOUT;
        }

    nrf_gpio_pin_write(31, 1); // DEBUGPIN31
    nrf_gpio_pin_write(31, 0); // DEBUGPIN31

    return DHT22_RCV_OK;
}

// bool
// DHT22_Read (dht22_data *out)
// {
//     out->rcv_response = DHT22_GetReadings (out);
//     if (out->rcv_response != DHT22_RCV_OK)
//     {
//         return false;
//     }
//     return true;
// }


static void
DHT22_DecodeReadings (dht22_data *out)
{
    uint8_t i = 0;

    for (; i < 8; i++)
    {
        out->hMSB <<= 1;
        if (out->bits[i] > 48)
            out->hMSB |= 1;
    }

    for (; i < 16; i++)
    {
        out->hLSB <<= 1;
        if (out->bits[i] > 48)
            out->hLSB |= 1;
    }

    for (; i < 24; i++)
    {
        out->tMSB <<= 1;
        if (out->bits[i] > 48)
            out->tMSB |= 1;
    }

    for (; i < 32; i++)
    {
        out->tLSB <<= 1;
        if (out->bits[i] > 48)
            out->tLSB |= 1;
    }

    for (; i < 40; i++)
    {
        out->parity_rcv <<= 1;
        if (out->bits[i] > 48)
            out->parity_rcv |= 1;
    }

    out->parity = out->hMSB + out->hLSB + out->tMSB + out->tLSB;

}

static uint16_t
DHT22_GetHumidity (dht22_data *out)
{
    return (out->hMSB << 8) | out->hLSB;
}

static uint16_t
DHT22_GetTemperature (dht22_data *out)
{
    return (out->tMSB << 8) | out->tLSB;
}

bool
DHT22_Read (dht22_data *out)
{
    out->rcv_response = DHT22_GetReadings (out);
    if (out->rcv_response != DHT22_RCV_OK)
    {
        return false;
    }

    DHT22_DecodeReadings (out);

    if (out->parity != out->parity_rcv)
    {
        out->rcv_response = DHT22_BAD_DATA;
        return false;
    }

    out->humidity = (float) DHT22_GetHumidity (out) / 10.0f;

    uint16_t temperature = DHT22_GetTemperature (out);
    out->temperature = ((float) (temperature & 0x7fff)) / 10.0f;

    if (temperature & 0x8000)
    {
        out->temperature = -out->temperature;
    }

    return true;
}
