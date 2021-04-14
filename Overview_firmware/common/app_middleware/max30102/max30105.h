/*************************************************** 
 This is a library written for the Maxim MAX30105 Optical Smoke Detector
 It should also work with the MAX30102. However, the MAX30102 does not have a Green LED.
 These sensors use I2C to communicate, as well as a single (optional)
 interrupt line that is not currently supported in this driver.
 
 Written by Peter Jansen and Nathan Seidle (SparkFun)
 BSD license, all text above must be included in any redistribution.
 *****************************************************/

#ifndef _MAX30105_H_
#define _MAX30105_H_

#include <stdint.h>
// LOG
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "SEGGER_RTT.h"
// HAL
#include "appHal_gpio.h"
#include "appHal_rtc.h"
#include "appHal_twi.h"
// DRV
#include "nrf_drv_twi.h"


#define MAX30105_ADDRESS          0x57 //7-bit I2C Address
#define _MAX30105_W_ADDRESS      ((MAX30105_ADDRESS <<1) & 0xFE)    
#define _MAX30105_R_ADDRESS      ((MAX30105_ADDRESS <<1) | 0x01)    
//Note that MAX30102 has the same I2C address and Part ID


// Status Registers
static const uint8_t MAX30105_INTSTAT1        = 0x00;
static const uint8_t MAX30105_INTSTAT2        = 0x01;
static const uint8_t MAX30105_INTENABLE1      = 0x02;
static const uint8_t MAX30105_INTENABLE2      = 0x03;

// FIFO Registers
static const uint8_t MAX30105_FIFOWRITEPTR    = 0x04;
static const uint8_t MAX30105_FIFOOVERFLOW    = 0x05;
static const uint8_t MAX30105_FIFOREADPTR     = 0x06;
static const uint8_t MAX30105_FIFODATA        = 0x07;

// Configuration Registers
static const uint8_t MAX30105_FIFOCONFIG      = 0x08;
static const uint8_t MAX30105_MODECONFIG      = 0x09;
static const uint8_t MAX30105_PARTICLECONFIG  = 0x0A;    // Note, sometimes listed as "SPO2" config in datasheet (pg. 11)
static const uint8_t MAX30105_LED1_PULSEAMP   = 0x0C;
static const uint8_t MAX30105_LED2_PULSEAMP   = 0x0D;
//static const uint8_t MAX30105_LED3_PULSEAMP = 0x0E; //No Green
static const uint8_t MAX30105_LED_PROX_AMP    = 0x10;
static const uint8_t MAX30105_MULTILEDCONFIG1 = 0x11;
static const uint8_t MAX30105_MULTILEDCONFIG2 = 0x12;

// Die Temperature Registers
static const uint8_t MAX30105_DIETEMPINT      = 0x1F;
static const uint8_t MAX30105_DIETEMPFRAC     = 0x20;
static const uint8_t MAX30105_DIETEMPCONFIG   = 0x21;

// Proximity Function Registers
static const uint8_t MAX30105_PROXINTTHRESH   = 0x30;

// Part ID Registers
static const uint8_t MAX30105_REVISIONID      = 0xFE;
static const uint8_t MAX30105_PARTID          = 0xFF;    // Should always be 0x15. Identical to MAX30102.

// MAX30105 Commands
// Interrupt configuration (pg 13, 14)
// static const uint8_t MAX30105_INT_A_FULL_MASK          = (uint8_t)~(0b10000000);
static const uint8_t MAX30105_INT_A_FULL_MASK          = (uint8_t) ~0x80;
static const uint8_t MAX30105_INT_A_FULL_ENABLE        = 0x80;
static const uint8_t MAX30105_INT_A_FULL_DISABLE       = 0x00;

// static const uint8_t MAX30105_INT_DATA_RDY_MASK        = (uint8_t)~(0b01000000);
static const uint8_t MAX30105_INT_DATA_RDY_MASK        = (uint8_t) ~0x40;
static const uint8_t MAX30105_INT_DATA_RDY_ENABLE      = 0x40;
static const uint8_t MAX30105_INT_DATA_RDY_DISABLE     = 0x00;

// static const uint8_t MAX30105_INT_ALC_OVF_MASK         = (uint8_t)~(0b00100000);
static const uint8_t MAX30105_INT_ALC_OVF_MASK         = (uint8_t) ~0x20;
static const uint8_t MAX30105_INT_ALC_OVF_ENABLE       = 0x20;
static const uint8_t MAX30105_INT_ALC_OVF_DISABLE      = 0x00;

// static const uint8_t MAX30105_INT_PROX_INT_MASK        = (uint8_t)~(0b00010000);
static const uint8_t MAX30105_INT_PROX_INT_MASK        = (uint8_t) ~0x10;
static const uint8_t MAX30105_INT_PROX_INT_ENABLE      = 0x10;
static const uint8_t MAX30105_INT_PROX_INT_DISABLE     = 0x00;

// static const uint8_t MAX30105_INT_DIE_TEMP_RDY_MASK    = (uint8_t)~(0b00000010);
static const uint8_t MAX30105_INT_DIE_TEMP_RDY_MASK    = (uint8_t) ~0x02;
static const uint8_t MAX30105_INT_DIE_TEMP_RDY_ENABLE  = 0x02;
static const uint8_t MAX30105_INT_DIE_TEMP_RDY_DISABLE = 0x00;

// static const uint8_t MAX30105_SAMPLEAVG_MASK   = (uint8_t)~(0b11100000);
static const uint8_t MAX30105_SAMPLEAVG_MASK   = (uint8_t) ~0xE0;
static const uint8_t MAX30105_SAMPLEAVG_1      = 0x00;
static const uint8_t MAX30105_SAMPLEAVG_2      = 0x20;
static const uint8_t MAX30105_SAMPLEAVG_4      = 0x40;
static const uint8_t MAX30105_SAMPLEAVG_8      = 0x60;
static const uint8_t MAX30105_SAMPLEAVG_16     = 0x80;
static const uint8_t MAX30105_SAMPLEAVG_32     = 0xA0;

static const uint8_t MAX30105_ROLLOVER_MASK    = 0xEF;
static const uint8_t MAX30105_ROLLOVER_ENABLE  = 0x10;
static const uint8_t MAX30105_ROLLOVER_DISABLE = 0x00;

static const uint8_t MAX30105_A_FULL_MASK      = 0xF0;

// Mode configuration commands (page 19)
static const uint8_t MAX30105_SHUTDOWN_MASK    = 0x7F;
static const uint8_t MAX30105_SHUTDOWN         = 0x80;
static const uint8_t MAX30105_WAKEUP           = 0x00;

static const uint8_t MAX30105_RESET_MASK       = 0xBF;
static const uint8_t MAX30105_RESET            = 0x40;

static const uint8_t MAX30105_MODE_MASK        = 0xF8;
static const uint8_t MAX30105_MODE_REDONLY     = 0x02; //Heart Rate
static const uint8_t MAX30105_MODE_REDIRONLY   = 0x03; //SPO2
static const uint8_t MAX30105_MODE_MULTILED    = 0x07; //Multi-LED

// Particle sensing configuration commands (pgs 19-20)
static const uint8_t MAX30105_ADCRANGE_MASK    = 0x9F;
static const uint8_t MAX30105_ADCRANGE_2048    = 0x00;
static const uint8_t MAX30105_ADCRANGE_4096    = 0x20;
static const uint8_t MAX30105_ADCRANGE_8192    = 0x40;
static const uint8_t MAX30105_ADCRANGE_16384   = 0x60;

static const uint8_t MAX30105_SAMPLERATE_MASK  = 0xE3;
static const uint8_t MAX30105_SAMPLERATE_50    = 0x00;
static const uint8_t MAX30105_SAMPLERATE_100   = 0x04;
static const uint8_t MAX30105_SAMPLERATE_200   = 0x08;
static const uint8_t MAX30105_SAMPLERATE_400   = 0x0C;
static const uint8_t MAX30105_SAMPLERATE_800   = 0x10;
static const uint8_t MAX30105_SAMPLERATE_1000  = 0x14;
static const uint8_t MAX30105_SAMPLERATE_1600  = 0x18;
static const uint8_t MAX30105_SAMPLERATE_3200  = 0x1C;

static const uint8_t MAX30105_PULSEWIDTH_MASK  = 0xFC;
static const uint8_t MAX30105_PULSEWIDTH_69    = 0x00;
static const uint8_t MAX30105_PULSEWIDTH_118   = 0x01;
static const uint8_t MAX30105_PULSEWIDTH_215   = 0x02;
static const uint8_t MAX30105_PULSEWIDTH_411   = 0x03;

//Multi-LED Mode configuration (pg 22)
static const uint8_t MAX30105_SLOT1_MASK       = 0xF8;
static const uint8_t MAX30105_SLOT2_MASK       = 0x8F;
static const uint8_t MAX30105_SLOT3_MASK       = 0xF8;
static const uint8_t MAX30105_SLOT4_MASK       = 0x8F;

static const uint8_t SLOT_NONE                 = 0x00;
static const uint8_t SLOT_RED_LED              = 0x01;
static const uint8_t SLOT_IR_LED               = 0x02;
//static const uint8_t SLOT_GREEN_LED          = 0x03;
//static const uint8_t SLOT_NONE_PILOT         = 0x04;
static const uint8_t SLOT_RED_PILOT            = 0x05;
static const uint8_t SLOT_IR_PILOT             = 0x06;
//static const uint8_t SLOT_GREEN_PILOT        = 0x07;

static const uint8_t MAX_30105_EXPECTEDPARTID = 0x15;


//I2C_BUFFER_LENGTH is defined in Wire.H
//#define I2C_BUFFER_LENGTH BUFFER_LENGTH

//The catch-all default is 32
#define I2C_BUFFER_LENGTH 32

uint32_t MAX30105_getRed(void); //Returns immediate red value
uint32_t MAX30105_getIR(void); //Returns immediate IR value
bool MAX30105_getRedIR(int32_t *data); //Returns immediate red and IR values

//  uint32_t MAX30105_getGreen(void); //Returns immediate green value
uint8_t MAX30105_safeCheck(uint8_t maxTimeToCheck); //Given a max amount of time, check for new data

// Configuration
void MAX30105_softReset(void);
void MAX30105_shutDown(void); 
void MAX30105_wakeUp(void); 

void MAX30105_setLEDMode(uint8_t mode);

void MAX30105_setADCRange(uint8_t adcRange);
void MAX30105_setSampleRate(uint8_t sampleRate);
void MAX30105_setPulseWidth(uint8_t pulseWidth);

void MAX30105_setPulseAmplitudeRed(uint8_t value);
void MAX30105_setPulseAmplitudeIR(uint8_t value);// void MAX30105_setPulseAmplitudeGreen(uint8_t value);
void MAX30105_setPulseAmplitudeProximity(uint8_t value);

void MAX30105_setProximityThreshold(uint8_t threshMSB);

//Multi-led configuration mode (page 22)
void MAX30105_enableSlot(uint8_t slotNumber, uint8_t device); //Given slot number, assign a device to slot
void MAX30105_disableSlots(void);
  
// Data Collection

//Interrupts (page 13, 14)
uint8_t MAX30105_getINT1(void); //Returns the main interrupt group
uint8_t MAX30105_getINT2(void); //Returns the temp ready interrupt
void MAX30105_enableAFULL(void); //Enable/disable individual interrupts
void MAX30105_disableAFULL(void);
void MAX30105_enableDATARDY(void);
void MAX30105_disableDATARDY(void);
void MAX30105_enableALCOVF(void);
void MAX30105_disableALCOVF(void);
void MAX30105_enablePROXINT(void);
void MAX30105_disablePROXINT(void);
void MAX30105_enableDIETEMPRDY(void);
void MAX30105_disableDIETEMPRDY(void);

//FIFO Configuration (page 18)
void MAX30105_setFIFOAverage(uint8_t samples);
void MAX30105_enableFIFORollover(void);
void MAX30105_disableFIFORollover(void);
void MAX30105_setFIFOAlmostFull(uint8_t samples);
  
//FIFO Reading
uint16_t MAX30105_check(void); //Checks for new data and fills FIFO
uint8_t MAX30105_available(void); //Tells caller how many new samples are available (head - tail)
void MAX30105_nextSample(void); //Advances the tail of the sense array
uint32_t MAX30105_getFIFORed(void); //Returns the FIFO sample pointed to by tail
uint32_t MAX30105_getFIFOIR(void); //Returns the FIFO sample pointed to by tail
//uint32_t MAX30105_getFIFOGreen(void); //Returns the FIFO sample pointed to by tail

uint8_t MAX30105_getWritePointer(void);
uint8_t MAX30105_getReadPointer(void);
void MAX30105_clearFIFO(void); //Sets the read/write pointers to zero

//Proximity Mode Interrupt Threshold
void MAX30105_setPROXINTTHRESH(uint8_t val);

// Die Temperature
float MAX30105_readTemperature(void);
float MAX30105_readTemperatureF(void);

// Detecting ID/Revision
uint8_t MAX30105_getRevisionID(void);
uint8_t MAX30105_readPartID(void);  

// Setup the IC with user selectable settings
void MAX30105_setup(uint8_t powerLevel, uint8_t sampleAverage, uint8_t ledMode, int sampleRate, int pulseWidth, int adcRange);

// Low-level I2C communication
uint8_t MAX30105_read8(uint8_t reg);
void MAX30105_write8(uint8_t reg, uint8_t value);
void MAX30105_read8_multi(uint8_t reg, uint8_t* dataDest, uint8_t dataSize);
void wait_xfer(void);
int MAX30105_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event);

void MAX30105_readRevisionID(void);

void MAX30105_bitMask(uint8_t reg, uint8_t mask, uint8_t thing);

#endif