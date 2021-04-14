#ifndef NODE_BOARD_H
#define NODE_BOARD_H

#define LED_1 13
#define PIN_LED_1 LED_1
#define PIN_LED_1_MASK (1 << PIN_LED_1)

#define LED_2 14
#define PIN_LED_2 LED_2
#define PIN_LED_2_MASK (1 << PIN_LED_2)

#define LED_3 15
#define PIN_LED_3 LED_3
#define PIN_LED_3_MASK (1 << PIN_LED_3)

#define LED_4 16
#define PIN_LED_4 LED_4
#define PIN_LED_4_MASK (1 << PIN_LED_4)

#define OUTS_MASK (PIN_LED_1_MASK | PIN_LED_2_MASK | PIN_LED_3_MASK | PIN_LED_4_MASK )

#define BUTTON_1 11
#define PIN_BUTTON_1 BUTTON_1
#define PIN_BUTTON_1_MASK (1 << PIN_BUTTON_1)

#define BUTTON_2 12
#define PIN_BUTTON_2 BUTTON_2
#define PIN_BUTTON_2_MASK (1 << PIN_BUTTON_2)

#define BUTTON_3 24
#define PIN_BUTTON_3 BUTTON_3
#define PIN_BUTTON_3_MASK (1 << PIN_BUTTON_3)

#define BUTTON_4 25
#define PIN_BUTTON_4 BUTTON_4
#define PIN_BUTTON_4_MASK (1 << PIN_BUTTON_4)

#define RX_PIN_NUMBER  8
#define TX_PIN_NUMBER  6
#define CTS_PIN_NUMBER 7
#define RTS_PIN_NUMBER 5
#define HWFC           true

#endif
