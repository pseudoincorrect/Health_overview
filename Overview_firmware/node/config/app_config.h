#ifndef APP_CONFIG_H__
#define APP_CONFIG_H__

/////////////////////////////////////////////////////////////////////
//
//  PRS - Peripheral Resource Sharing 
//
/////////////////////////////////////////////////////////////////////
//#ifndef NRFX_PRS_ENABLED
//#define NRFX_PRS_ENABLED 1
//#endif
//
//// NRFX_PRS_BOX_0_ENABLED  - Enables box 0 in the module.
//#ifndef NRFX_PRS_BOX_0_ENABLED
//#define NRFX_PRS_BOX_0_ENABLED 0
//#endif
//
//// NRFX_PRS_BOX_1_ENABLED  - Enables box 1 in the module.
//#ifndef NRFX_PRS_BOX_1_ENABLED
//#define NRFX_PRS_BOX_1_ENABLED 0
//#endif
//
//// NRFX_PRS_BOX_2_ENABLED  - Enables box 2 in the module.
//#ifndef NRFX_PRS_BOX_2_ENABLED
//#define NRFX_PRS_BOX_2_ENABLED 0
//#endif
//
//// NRFX_PRS_BOX_3_ENABLED  - Enables box 3 in the module.
//#ifndef NRFX_PRS_BOX_3_ENABLED
//#define NRFX_PRS_BOX_3_ENABLED 0
//#endif
//
//// NRFX_PRS_BOX_4_ENABLED  - Enables box 4 in the module.
//#ifndef NRFX_PRS_BOX_4_ENABLED
//#define NRFX_PRS_BOX_4_ENABLED 1
//#endif

/////////////////////////////////////////////////////////////////////
//
//  TIMER
//
/////////////////////////////////////////////////////////////////////
#ifndef TIMER_ENABLED
#define TIMER_ENABLED 1
#endif

// TIMER0_ENABLED - Enable TIMER0 instance
#ifndef TIMER0_ENABLED
#define TIMER0_ENABLED 1
#endif

// TIMER1_ENABLED - Enable TIMER1 instance
#ifndef TIMER1_ENABLED
#define TIMER1_ENABLED 0
#endif

// TIMER2_ENABLED - Enable TIMER2 instance
#ifndef TIMER2_ENABLED
#define TIMER2_ENABLED 1
#endif

// TIMER3_ENABLED - Enable TIMER3 instance
#ifndef TIMER3_ENABLED
#define TIMER3_ENABLED 0
#endif

// TIMER4_ENABLED - Enable TIMER4 instance
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

// NRFX_TIMER0_ENABLED - Enable TIMER0 instance
#ifndef NRFX_TIMER0_ENABLED
#define NRFX_TIMER0_ENABLED 0
#endif

// NRFX_TIMER1_ENABLED - Enable TIMER1 instance
#ifndef NRFX_TIMER1_ENABLED
#define NRFX_TIMER1_ENABLED 0
#endif

// NRFX_TIMER2_ENABLED - Enable TIMER2 instance
#ifndef NRFX_TIMER2_ENABLED
#define NRFX_TIMER2_ENABLED 1
#endif

// NRFX_TIMER3_ENABLED - Enable TIMER3 instance
#ifndef NRFX_TIMER3_ENABLED
#define NRFX_TIMER3_ENABLED 0
#endif

// NRFX_TIMER4_ENABLED - Enable TIMER4 instance
#ifndef NRFX_TIMER4_ENABLED
#define NRFX_TIMER4_ENABLED 0
#endif

// 0 = 16 MHz      1 = 8 MHz      2 = 4 MHz 
// 3 = 2 MHz       4 = 1 MHz      5 = 500 kHz 
// 6 = 250 kHz     7 = 125 kHz    8 = 62.5 kHz    9 = 31.25 kHz 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_FREQUENCY
#define NRFX_TIMER_DEFAULT_CONFIG_FREQUENCY 0
#endif

// NRFX_TIMER_DEFAULT_CONFIG_MODE - Timer mode or operation
// 0 = Timer  1 = Counter 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_MODE
#define NRFX_TIMER_DEFAULT_CONFIG_MODE 0
#endif

// NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH - Timer counter bit width
// 0 = 16 bit      1 = 8 bit 
// 2 = 24 bit      3 = 32 bit 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH
#define NRFX_TIMER_DEFAULT_CONFIG_BIT_WIDTH 0
#endif

/////////////////////////////////////////////////////////////////////
//
//  Timer Default Settings
//
/////////////////////////////////////////////////////////////////////
// NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY - Interrupt priority
// 0 = 0 (highest)  7 = 7 
#ifndef NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// TIMER_DEFAULT_CONFIG_FREQUENCY - Timer frequency if in Timer mode
// 0 = 16 MHz 
// 1 = 8 MHz       2 = 4 MHz          3 = 2 MHz 
// 4 = 1 MHz       5 = 500 kHz        6 = 250 kHz 
// 7 = 125 kHz     8 = 62.5 kHz       9 = 31.25 kHz 
#ifndef TIMER_DEFAULT_CONFIG_FREQUENCY
#define TIMER_DEFAULT_CONFIG_FREQUENCY 0
#endif

// TIMER_DEFAULT_CONFIG_MODE - Timer mode or operation
// 0 = Timer 
// 1 = Counter 
#ifndef TIMER_DEFAULT_CONFIG_MODE
#define TIMER_DEFAULT_CONFIG_MODE 0
#endif

// TIMER_DEFAULT_CONFIG_BIT_WIDTH - Timer counter bit width
// 0 = 16 bit      1 = 8 bit 
// 2 = 24 bit      3 = 32 bit 
#ifndef TIMER_DEFAULT_CONFIG_BIT_WIDTH
#define TIMER_DEFAULT_CONFIG_BIT_WIDTH 3
#endif

// TIMER_DEFAULT_CONFIG_IRQ_PRIORITY - Interrupt priority
// Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// 0 = 0 (highest)  7 = 7 
#ifndef TIMER_DEFAULT_CONFIG_IRQ_PRIORITY
#define TIMER_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif


/////////////////////////////////////////////////////////////////////
//
//  RTC
//
/////////////////////////////////////////////////////////////////////
#ifndef NRFX_CLOCK_ENABLED
#define NRFX_CLOCK_ENABLED 1
#endif
// NRFX_CLOCK_CONFIG_LF_SRC  - LF Clock Source
// 0 = RC 
// 1 = XTAL 
#ifndef NRFX_CLOCK_CONFIG_LF_SRC
#define NRFX_CLOCK_CONFIG_LF_SRC 0
#endif

// NRFX_CLOCK_CONFIG_IRQ_PRIORITY  - Interrupt priority
// 0 = 0 (highest)  7 = 7 
#ifndef NRFX_CLOCK_CONFIG_IRQ_PRIORITY
#define NRFX_CLOCK_CONFIG_IRQ_PRIORITY 6
#endif

#ifndef NRFX_RTC_ENABLED
#define NRFX_RTC_ENABLED 1
#endif

// NRFX_RTC0_ENABLED  - Enable RTC0 instance
#ifndef NRFX_RTC0_ENABLED
#define NRFX_RTC0_ENABLED 0
#endif

// NRFX_RTC1_ENABLED  - Enable RTC1 instance
#ifndef NRFX_RTC1_ENABLED
#define NRFX_RTC1_ENABLED 0
#endif

// NRFX_RTC2_ENABLED  - Enable RTC2 instance
#ifndef NRFX_RTC2_ENABLED
#define NRFX_RTC2_ENABLED 0
#endif

// NRFX_RTC_MAXIMUM_LATENCY_US - Maximum possible time[us] in highest priority interrupt 
#ifndef NRFX_RTC_MAXIMUM_LATENCY_US
#define NRFX_RTC_MAXIMUM_LATENCY_US 2000
#endif

// NRFX_RTC_DEFAULT_CONFIG_FREQUENCY - Frequency  <16-32768> 
#ifndef NRFX_RTC_DEFAULT_CONFIG_FREQUENCY
#define NRFX_RTC_DEFAULT_CONFIG_FREQUENCY 32768
#endif

// NRFX_RTC_DEFAULT_CONFIG_RELIABLE  - Ensures safe compare event triggering
#ifndef NRFX_RTC_DEFAULT_CONFIG_RELIABLE
#define NRFX_RTC_DEFAULT_CONFIG_RELIABLE 0
#endif

// NRFX_RTC_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// 0 = 0 (highest)  7 = 7 
#ifndef NRFX_RTC_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_RTC_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// NRF_CLOCK_ENABLED
#ifndef NRF_CLOCK_ENABLED
#define NRF_CLOCK_ENABLED 1
#endif

// CLOCK_CONFIG_LF_SRC  - LF Clock Source
// 0 = RC 
// 1 = XTAL 
#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 0
#endif

// CLOCK_CONFIG_IRQ_PRIORITY  - Interrupt priority
// 0 = 0 (highest)  7 = 7 
#ifndef CLOCK_CONFIG_IRQ_PRIORITY
#define CLOCK_CONFIG_IRQ_PRIORITY 6
#endif

// RTC_ENABLED - nrf_drv_rtc - RTC peripheral driver - legacy layer
#ifndef RTC_ENABLED
#define RTC_ENABLED 1
#endif

// RTC_DEFAULT_CONFIG_FREQUENCY - Frequency  <16-32768> 
#ifndef RTC_DEFAULT_CONFIG_FREQUENCY
#define RTC_DEFAULT_CONFIG_FREQUENCY 32768
#endif

// RTC_DEFAULT_CONFIG_RELIABLE  - Ensures safe compare event triggering
#ifndef RTC_DEFAULT_CONFIG_RELIABLE
#define RTC_DEFAULT_CONFIG_RELIABLE 0
#endif

// RTC_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// 0 = 0 (highest)  7 = 7 
#ifndef RTC_DEFAULT_CONFIG_IRQ_PRIORITY
#define RTC_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// RTC0_ENABLED  - Enable RTC0 instance
#ifndef RTC0_ENABLED
#define RTC0_ENABLED 1
#endif

// RTC1_ENABLED  - Enable RTC1 instance
#ifndef RTC1_ENABLED
#define RTC1_ENABLED 0
#endif

// RTC2_ENABLED  - Enable RTC2 instance
#ifndef RTC2_ENABLED
#define RTC2_ENABLED 0
#endif

// NRF_MAXIMUM_LATENCY_US - Maximum possible time[us] in highest priority interrupt 
#ifndef NRF_MAXIMUM_LATENCY_US
#define NRF_MAXIMUM_LATENCY_US 2000
#endif

/////////////////////////////////////////////////////////////////////
//
//  TWI
//
/////////////////////////////////////////////////////////////////////
// TWI_ENABLED - nrf_drv_twi - TWI/TWIM peripheral driver - legacy layer
#ifndef TWI_ENABLED
#define TWI_ENABLED 1
#endif

// TWI_DEFAULT_CONFIG_FREQUENCY - Frequency
// 26738688 = 100k 
// 67108864 = 250k 
// 104857600 = 400k 
#ifndef TWI_DEFAULT_CONFIG_FREQUENCY
#define TWI_DEFAULT_CONFIG_FREQUENCY 26738688
#endif

// TWI_DEFAULT_CONFIG_CLR_BUS_INIT - Enables bus clearing procedure during init
#ifndef TWI_DEFAULT_CONFIG_CLR_BUS_INIT
#define TWI_DEFAULT_CONFIG_CLR_BUS_INIT 0
#endif

// TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT - Enables bus holding after uninit
#ifndef TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT
#define TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT 0
#endif

// TWI_DEFAULT_CONFIG_IRQ_PRIORITY - Interrupt priority
// Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// 0 = 0 (highest) to 7 = 7 

#ifndef TWI_DEFAULT_CONFIG_IRQ_PRIORITY
#define TWI_DEFAULT_CONFIG_IRQ_PRIORITY 6
#endif

// TWI0_ENABLED - Enable TWI0 instance
#ifndef TWI0_ENABLED
#define TWI0_ENABLED 0
#endif

// TWI0_USE_EASY_DMA - Use EasyDMA (if present)
#ifndef TWI0_USE_EASY_DMA
#define TWI0_USE_EASY_DMA 0
#endif

// TWI1_ENABLED - Enable TWI1 instance
#ifndef TWI1_ENABLED
#define TWI1_ENABLED 1
#endif

// TWI1_USE_EASY_DMA - Use EasyDMA (if present)
#ifndef TWI1_USE_EASY_DMA
#define TWI1_USE_EASY_DMA 1
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
#define NRFX_WDT_CONFIG_RELOAD_VALUE 30000
#endif

// NRFX_WDT_CONFIG_IRQ_PRIORITY  - Interrupt priority
// 0 = 0 (highest) to 7 = 7 
#ifndef NRFX_WDT_CONFIG_IRQ_PRIORITY
#define NRFX_WDT_CONFIG_IRQ_PRIORITY 6
#endif

#endif