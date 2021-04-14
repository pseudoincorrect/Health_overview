#include "gateway_mock.h"


const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(2);
sensors_t mock;


void mock_uart_data (void) 
{
    char msg_str_raw[32];
    get_mock_uart_data(msg_str_raw);
    UART_PRINTF("%s\n\r", msg_str_raw);
    SEGGER_RTT_printf(0, "%s\n\r", msg_str_raw);
}

// Timer used to generate mock data
void mock_timer_handler(nrf_timer_event_t event_type, void* p_context)
{
    #if(MOCK_UART_DATA)
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
                mock_uart_data();
                bsp_board_led_invert(BSP_BOARD_LED_1);
            break;
        default:
            break;
    }
    #endif
}

// Function for initializing the Mock timer
void init_mock(void) 
{
    uint32_t time_ms = 3000; //Time(ms) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
   //Configure TIMER_LED for generating simple light effect - 
   //leds on board will invert his state one after the other.
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, mock_timer_handler);
    APP_ERROR_CHECK(err_code);
    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, time_ms);
    nrf_drv_timer_extended_compare(
            &TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, 
            NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&TIMER_LED);
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void init_mock_uart_data (void)

{
    n_sensors_t* n_s = &mock.origin.node;
    srand(time(NULL));
    n_s->oxymetry = 0;
    n_s->temperature = 0;
    n_s->humidity = 0;
    n_s->heart_rate = 0;
    n_s->dehydration = 0;
    n_s->reserved1 = 0;
    n_s->reserved2 = 0;
}

////////////////////////////////////////////////////////////////////////////////
// 
//
void get_mock_uart_data (char* msg_str_raw) 
{
    fifo_data_packet_t fdp;
    uint8_t randomNb = rand();
    int i;

    n_sensors_t* n_s = &mock.origin.node;
    
    if (randomNb & 0b00000001) n_s->oxymetry    = (n_s->oxymetry    < 100) ? n_s->oxymetry    + 3 : 100;
    else                       n_s->oxymetry    = (n_s->oxymetry    > 10 ) ? n_s->oxymetry    - 3 : 10;
    if (randomNb & 0b00000010) n_s->temperature = (n_s->temperature < 100) ? n_s->temperature + 3 : 100;
    else                       n_s->temperature = (n_s->temperature > 10 ) ? n_s->temperature - 3 : 10;
    if (randomNb & 0b00000100) n_s->humidity    = (n_s->humidity    < 100) ? n_s->humidity    + 3 : 100;
    else                       n_s->humidity    = (n_s->humidity    > 10 ) ? n_s->humidity    - 3 : 10;
    if (randomNb & 0b00001000) n_s->heart_rate  = (n_s->heart_rate  < 100) ? n_s->heart_rate  + 3 : 100;
    else                       n_s->heart_rate  = (n_s->heart_rate  > 10 ) ? n_s->heart_rate  - 3 : 10;
    if (randomNb & 0b00010000) n_s->dehydration = (n_s->dehydration < 100) ? n_s->dehydration + 3 : 100;
    else                       n_s->dehydration = (n_s->dehydration > 10 ) ? n_s->dehydration - 3 : 10;
    if (randomNb & 0b00100000) n_s->reserved1   = (n_s->reserved1   < 100) ? n_s->reserved1   + 3 : 100;
    else                       n_s->reserved1   = (n_s->reserved1   > 10 ) ? n_s->reserved1   - 3 : 10;
    if (randomNb & 0b01000000) n_s->reserved2   = (n_s->reserved2   < 100) ? n_s->reserved2   + 3 : 100;
    else                       n_s->reserved2   = (n_s->reserved2   > 10 ) ? n_s->reserved2   - 3 : 10;

    mock.addr.a[0] = 0;
    mock.addr.a[1] = 0;
    mock.addr.a[2] = 0;
    mock.addr.a[3] = 0;
    mock.addr.a[4] = 0;
    mock.addr.a[5] = (randomNb % 8) + 1;

    memset(&fdp, 0, sizeof(fifo_data_packet_t));
    fdp.type = N_G_SENSORS;
    memcpy(&(fdp.content.sensors), &mock, sizeof(sensors_t));

    sprintf(msg_str_raw,
        "%04x%02x%02x%02x%02x%02x%02x%04x%04x%04x%04x%04x%04x%04x",
        fdp.type,
        fdp.content.sensors.addr.a[0], 
        fdp.content.sensors.addr.a[1], 
        fdp.content.sensors.addr.a[2],
        fdp.content.sensors.addr.a[3], 
        fdp.content.sensors.addr.a[4], 
        fdp.content.sensors.addr.a[5],
        fdp.content.sensors.origin.node.temperature,
        fdp.content.sensors.origin.node.heart_rate,
        fdp.content.sensors.origin.node.oxymetry,
        fdp.content.sensors.origin.node.humidity,
        fdp.content.sensors.origin.node.dehydration,
        fdp.content.sensors.origin.node.reserved1,
        fdp.content.sensors.origin.node.reserved2);
}