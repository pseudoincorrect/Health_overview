#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__


/////////////////////////////////////////////////////////////////////
//
//  PRS
//
/////////////////////////////////////////////////////////////////////
#ifndef NRFX_PRS_ENABLED
#define NRFX_PRS_ENABLED 0
#endif

// NRFX_PRS_BOX_0_ENABLED  - Enables box 0 in the module.
#ifndef NRFX_PRS_BOX_0_ENABLED
#define NRFX_PRS_BOX_0_ENABLED 0
#endif

// NRFX_PRS_BOX_1_ENABLED  - Enables box 1 in the module.
#ifndef NRFX_PRS_BOX_1_ENABLED
#define NRFX_PRS_BOX_1_ENABLED 0
#endif

// NRFX_PRS_BOX_2_ENABLED  - Enables box 2 in the module.
#ifndef NRFX_PRS_BOX_2_ENABLED
#define NRFX_PRS_BOX_2_ENABLED 0
#endif

// NRFX_PRS_BOX_3_ENABLED  - Enables box 3 in the module.
#ifndef NRFX_PRS_BOX_3_ENABLED
#define NRFX_PRS_BOX_3_ENABLED 0
#endif

// NRFX_PRS_BOX_4_ENABLED  - Enables box 4 in the module.
#ifndef NRFX_PRS_BOX_4_ENABLED
#define NRFX_PRS_BOX_4_ENABLED 0
#endif

// <e> NRFX_UARTE_ENABLED - nrfx_uarte - UARTE peripheral driver
//==========================================================
#ifndef NRFX_UARTE_ENABLED
#define NRFX_UARTE_ENABLED 1
#endif

// <o> NRFX_UARTE0_ENABLED - Enable UARTE0 instance 
#ifndef NRFX_UARTE0_ENABLED
#define NRFX_UARTE0_ENABLED 0
#endif

// <o> NRFX_UARTE1_ENABLED - Enable UARTE1 instance 
#ifndef NRFX_UARTE1_ENABLED
#define NRFX_UARTE1_ENABLED 0
#endif

// <o> NRFX_UARTE_DEFAULT_CONFIG_HWFC  - Hardware Flow Control
// <0=> Disabled 
// <1=> Enabled 
#ifndef NRFX_UARTE_DEFAULT_CONFIG_HWFC
#define NRFX_UARTE_DEFAULT_CONFIG_HWFC 0
#endif

// <o> NRFX_UARTE_DEFAULT_CONFIG_PARITY  - Parity
// <0=> Excluded 
// <14=> Included 
#ifndef NRFX_UARTE_DEFAULT_CONFIG_PARITY
#define NRFX_UARTE_DEFAULT_CONFIG_PARITY 0
#endif

// <o> NRFX_UARTE_DEFAULT_CONFIG_BAUDRATE  - Default Baudrate
// <30801920=> 115200 baud 
#ifndef NRFX_UARTE_DEFAULT_CONFIG_BAUDRATE
#define NRFX_UARTE_DEFAULT_CONFIG_BAUDRATE 30801920
#endif

// <o> NRFX_UARTE_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// <0=> 0 (highest)  // <7=> 7 
#ifndef NRFX_UARTE_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_UARTE_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// <e> NRFX_UARTE_CONFIG_LOG_ENABLED - Enables logging in the module.
//==========================================================
#ifndef NRFX_UARTE_CONFIG_LOG_ENABLED
#define NRFX_UARTE_CONFIG_LOG_ENABLED 0
#endif

// <o> NRFX_UARTE_CONFIG_LOG_LEVEL  - Default Severity level
#ifndef NRFX_UARTE_CONFIG_LOG_LEVEL
#define NRFX_UARTE_CONFIG_LOG_LEVEL 3
#endif

// <o> NRFX_UARTE_CONFIG_INFO_COLOR  - ANSI escape code prefix.
#ifndef NRFX_UARTE_CONFIG_INFO_COLOR
#define NRFX_UARTE_CONFIG_INFO_COLOR 0
#endif

// <o> NRFX_UARTE_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
#ifndef NRFX_UARTE_CONFIG_DEBUG_COLOR
#define NRFX_UARTE_CONFIG_DEBUG_COLOR 0
#endif

// </e>
// </e>
// <e> NRFX_UART_ENABLED - nrfx_uart - UART peripheral driver
//==========================================================
#ifndef NRFX_UART_ENABLED
#define NRFX_UART_ENABLED 1
#endif

// <o> NRFX_UART0_ENABLED - Enable UART0 instance 
#ifndef NRFX_UART0_ENABLED
#define NRFX_UART0_ENABLED 0
#endif

// <o> NRFX_UART_DEFAULT_CONFIG_HWFC  - Hardware Flow Control
// <0=> Disabled 
// <1=> Enabled 
#ifndef NRFX_UART_DEFAULT_CONFIG_HWFC
#define NRFX_UART_DEFAULT_CONFIG_HWFC 0
#endif

// <o> NRFX_UART_DEFAULT_CONFIG_PARITY  - Parity
// <0=> Excluded 
// <14=> Included 
#ifndef NRFX_UART_DEFAULT_CONFIG_PARITY
#define NRFX_UART_DEFAULT_CONFIG_PARITY 0
#endif

// <o> NRFX_UART_DEFAULT_CONFIG_BAUDRATE  - Default Baudrate
// <30924800=> 115200 baud 
#ifndef NRFX_UART_DEFAULT_CONFIG_BAUDRATE
#define NRFX_UART_DEFAULT_CONFIG_BAUDRATE 30924800
#endif

// <o> NRFX_UART_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// <0=> 0 (highest)  // <7=> 7 
#ifndef NRFX_UART_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_UART_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// <e> NRFX_UART_CONFIG_LOG_ENABLED - Enables logging in the module.
//==========================================================
#ifndef NRFX_UART_CONFIG_LOG_ENABLED
#define NRFX_UART_CONFIG_LOG_ENABLED 0
#endif


// <o> NRFX_UART_CONFIG_LOG_LEVEL  - Default Severity level
#ifndef NRFX_UART_CONFIG_LOG_LEVEL
#define NRFX_UART_CONFIG_LOG_LEVEL 3
#endif

// <o> NRFX_UART_CONFIG_INFO_COLOR  - ANSI escape code prefix.
#ifndef NRFX_UART_CONFIG_INFO_COLOR
#define NRFX_UART_CONFIG_INFO_COLOR 0
#endif

// <o> NRFX_UART_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.
#ifndef NRFX_UART_CONFIG_DEBUG_COLOR
#define NRFX_UART_CONFIG_DEBUG_COLOR 0
#endif

// </e>
// </e>
// <e> UART_ENABLED - nrf_drv_uart - UART/UARTE peripheral driver - legacy layer
//==========================================================
#ifndef UART_ENABLED
#define UART_ENABLED 1
#endif

// <o> UART_DEFAULT_CONFIG_HWFC  - Hardware Flow Control
// <0=> Disabled 
// <1=> Enabled 
#ifndef UART_DEFAULT_CONFIG_HWFC
#define UART_DEFAULT_CONFIG_HWFC 0
#endif

// <o> UART_DEFAULT_CONFIG_PARITY  - Parity
// <0=> Excluded 
// <14=> Included 
#ifndef UART_DEFAULT_CONFIG_PARITY
#define UART_DEFAULT_CONFIG_PARITY 0
#endif

// <o> UART_DEFAULT_CONFIG_BAUDRATE  - Default Baudrate
// <30801920=> 115200 baud 
#ifndef UART_DEFAULT_CONFIG_BAUDRATE
#define UART_DEFAULT_CONFIG_BAUDRATE 30801920
#endif

// <o> UART_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest)  // <7=> 7 
#ifndef UART_DEFAULT_CONFIG_IRQ_PRIORITY
#define UART_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// <q> UART_EASY_DMA_SUPPORT  - Driver supporting EasyDMA
#ifndef UART_EASY_DMA_SUPPORT
#define UART_EASY_DMA_SUPPORT 1
#endif

// <q> UART_LEGACY_SUPPORT  - Driver supporting Legacy mode
#ifndef UART_LEGACY_SUPPORT
#define UART_LEGACY_SUPPORT 1
#endif

// <e> UART0_ENABLED - Enable UART0 instance
//==========================================================
#ifndef UART0_ENABLED
#define UART0_ENABLED 0
#endif

// <q> UART0_CONFIG_USE_EASY_DMA  - Default setting for using EasyDMA
#ifndef UART0_CONFIG_USE_EASY_DMA
#define UART0_CONFIG_USE_EASY_DMA 1
#endif

// </e>
// <e> UART1_ENABLED - Enable UART1 instance
//==========================================================
#ifndef UART1_ENABLED
#define UART1_ENABLED 1
#endif

// </e>
// </e>
// </h> 
//==========================================================
// <h> nRF_Libraries 
// <q> APP_FIFO_ENABLED  - app_fifo - Software FIFO implementation
#ifndef APP_FIFO_ENABLED
#define APP_FIFO_ENABLED 1
#endif

// <e> APP_UART_ENABLED - app_uart - UART driver
//==========================================================
#ifndef APP_UART_ENABLED
#define APP_UART_ENABLED 1
#endif

// <o> APP_UART_DRIVER_INSTANCE  - UART instance used
// <0=> 0 
#ifndef APP_UART_DRIVER_INSTANCE
#define APP_UART_DRIVER_INSTANCE 1
#endif

#ifndef RETARGET_ENABLED
#define RETARGET_ENABLED 1
#endif

#ifndef NRF_FPRINTF_ENABLED
#define NRF_FPRINTF_ENABLED 1
#endif

/////////////////////////////////////////////////////////////////////
//
//  TIMER
//
/////////////////////////////////////////////////////////////////////
#ifndef TIMER_ENABLED
#define TIMER_ENABLED 1
#endif

// TIMER0_ENABLED  - Enable TIMER0 instance
#ifndef TIMER0_ENABLED
#define TIMER0_ENABLED 1
#endif

// TIMER1_ENABLED  - Enable TIMER1 instance
#ifndef TIMER1_ENABLED
#define TIMER1_ENABLED 0
#endif

// TIMER2_ENABLED  - Enable TIMER2 instance
#ifndef TIMER2_ENABLED
#define TIMER2_ENABLED 1
#endif

// TIMER3_ENABLED  - Enable TIMER3 instance
#ifndef TIMER3_ENABLED
#define TIMER3_ENABLED 0
#endif

// TIMER4_ENABLED  - Enable TIMER4 instance
#ifndef TIMER4_ENABLED
#define TIMER4_ENABLED 0
#endif

/////////////////////////////////////////////////////////////////////
//
//  NRFX_TIMER
//
/////////////////////////////////////////////////////////////////////
#ifndef NRFX_TIMER_ENABLED
#define NRFX_TIMER_ENABLED 1
#endif

// NRFX_TIMER0_ENABLED  - Enable TIMER0 instance
#ifndef NRFX_TIMER0_ENABLED
#define NRFX_TIMER0_ENABLED 0
#endif

// NRFX_TIMER1_ENABLED  - Enable TIMER1 instance
#ifndef NRFX_TIMER1_ENABLED
#define NRFX_TIMER1_ENABLED 0
#endif

// NRFX_TIMER2_ENABLED  - Enable TIMER2 instance
#ifndef NRFX_TIMER2_ENABLED
#define NRFX_TIMER2_ENABLED 1
#endif

// NRFX_TIMER3_ENABLED  - Enable TIMER3 instance
#ifndef NRFX_TIMER3_ENABLED
#define NRFX_TIMER3_ENABLED 0
#endif

// NRFX_TIMER4_ENABLED  - Enable TIMER4 instance
#ifndef NRFX_TIMER4_ENABLED
#define NRFX_TIMER4_ENABLED 0
#endif

// <0=> 16 MHz      <1=> 8 MHz      <2=> 4 MHz 
// <3=> 2 MHz       <4=> 1 MHz      <5=> 500 kHz 
// <6=> 250 kHz     <7=> 125 kHz    <8=> 62.5 kHz    <9=> 31.25 kHz 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_FREQUENCY
#define NRFX_TIMER_DEFAULT_CONFIG_FREQUENCY 0
#endif

// NRFX_TIMER_DEFAULT_CONFIG_MODE  - Timer mode or operation
// <0=> Timer  <1=> Counter 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_MODE
#define NRFX_TIMER_DEFAULT_CONFIG_MODE 0
#endif

// NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH  - Timer counter bit width
// <0=> 16 bit      <1=> 8 bit 
// <2=> 24 bit      <3=> 32 bit 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH
#define NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH 0
#endif

/////////////////////////////////////////////////////////////////////
//
//  Timer Default Settings
//
/////////////////////////////////////////////////////////////////////
// NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// <0=> 0 (highest)  <7=> 7 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// TIMER_DEFAULT_CONFIG_FREQUENCY  - Timer frequency if in Timer mode
// <0=> 16 MHz 
// <1=> 8 MHz       <2=> 4 MHz          <3=> 2 MHz 
// <4=> 1 MHz       <5=> 500 kHz        <6=> 250 kHz 
// <7=> 125 kHz     <8=> 62.5 kHz       <9=> 31.25 kHz 
#ifndef TIMER_DEFAULT_CONFIG_FREQUENCY
#define TIMER_DEFAULT_CONFIG_FREQUENCY 0
#endif

// TIMER_DEFAULT_CONFIG_MODE  - Timer mode or operation
// <0=> Timer 
// <1=> Counter 
#ifndef TIMER_DEFAULT_CONFIG_MODE
#define TIMER_DEFAULT_CONFIG_MODE 0
#endif

// TIMER_DEFAULT_CONFIG_BIT_WIDTH  - Timer counter bit width
// <0=> 16 bit      <1=> 8 bit 
// <2=> 24 bit      <3=> 32 bit 
#ifndef TIMER_DEFAULT_CONFIG_BIT_WIDTH
#define TIMER_DEFAULT_CONFIG_BIT_WIDTH 3
#endif

// TIMER_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest)  <7=> 7 
#ifndef TIMER_DEFAULT_CONFIG_IRQ_PRIORITY
#define TIMER_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

/////////////////////////////////////////////////////////////////////
//
//  WDT
//
/////////////////////////////////////////////////////////////////////
// NRFX_WDT_ENABLED - nrfx_wdt - WDT peripheral driver
#ifndef NRFX_WDT_ENABLED
#define NRFX_WDT_ENABLED 1
#endif

// NRFX_WDT_CONFIG_BEHAVIOUR  - WDT behavior in CPU SLEEP or HALT mode
// 1 = Run in SLEEP, Pause in HALT 
// 8 = Pause in SLEEP, Run in HALT 
// 9 = Run in SLEEP and HALT 
// 0 = Pause in SLEEP and HALT 
#ifndef NRFX_WDT_CONFIG_BEHAVIOUR
#define NRFX_WDT_CONFIG_BEHAVIOUR 1
#endif

// NRFX_WDT_CONFIG_RELOAD_VALUE - Reload value  15-4294967295> 
#ifndef NRFX_WDT_CONFIG_RELOAD_VALUE
#define NRFX_WDT_CONFIG_RELOAD_VALUE 7000
#endif

// NRFX_WDT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// 0 = 0 (highest) to 7 = 7 
#ifndef NRFX_WDT_CONFIG_IRQ_PRIORITY
#define NRFX_WDT_CONFIG_IRQ_PRIORITY 6
#endif

/////////////////////////////////////////////////////////////////////
//
//  LOG
//
/////////////////////////////////////////////////////////////////////
#ifndef NRF_LOG_ENABLED
#define NRF_LOG_ENABLED 1
#endif

#endif
