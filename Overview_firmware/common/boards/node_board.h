#ifndef NODE_BOARD_H
#define NODE_BOARD_H
#include "nrf_gpio.h"

#define PIN_OXI_INT     NRF_GPIO_PIN_MAP(1, 8)
#define PIN_BUTTON      NRF_GPIO_PIN_MAP(1, 6)
#define PIN_MOTOR       23
#define PIN_LEDR        20
#define PIN_DBG_1       13
#define PIN_DBG_2       NRF_GPIO_PIN_MAP(1, 0)
#define PIN_LEDG        NRF_GPIO_PIN_MAP(1, 7)
#define RX_PIN_NUMBER   8
#define TX_PIN_NUMBER   6
#define CTS_PIN_NUMBER  7
#define RTS_PIN_NUMBER  5
#define HWFC            true
#define TWI_SCL_PIN     27
#define TWI_SDA_PIN     26

#endif
