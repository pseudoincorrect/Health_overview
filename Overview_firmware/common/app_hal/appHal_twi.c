///////////////////////////////////////////////////////////////////////////////
//    INCLUDED FILES
///////////////////////////////////////////////////////////////////////////////

#include "appHal_twi.h"

///////////////////////////////////////////////////////////////////////////////
//    Private variables
///////////////////////////////////////////////////////////////////////////////

static nrf_drv_twi_t g_twi = NRF_DRV_TWI_INSTANCE(1);
static twi_event_t g_twi_event;

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////

static void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch(p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            switch(p_event->xfer_desc.type)
            {
                case NRF_DRV_TWI_XFER_TX:
                    g_twi_event.xfer_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXTX:
                    g_twi_event.xfer_done = true;
                    break;
                case NRF_DRV_TWI_XFER_RX:
                    g_twi_event.xfer_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXRX:
                    g_twi_event.xfer_done = true;
                    break;
                default:
                    break;
            }
            break;
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            break;
        case NRF_DRV_TWI_EVT_DATA_NACK:
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
//    Public Functions
///////////////////////////////////////////////////////////////////////////////

void appHal_twi_init (void)
{
    ret_code_t err_code;

    g_twi_event.xfer_done = false;

    const nrf_drv_twi_config_t twi_config =
    {
        .scl                = TWI_SCL_PIN,
        .sda                = TWI_SDA_PIN,
        .frequency          = NRF_DRV_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&g_twi, &twi_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&g_twi);
}

nrf_drv_twi_t* appHal_twi_get_instance (void)
{
    return (&g_twi);
}

twi_event_t* appHal_twi_get_event_instance (void)
{
    return (&g_twi_event);
}