/***************************************************
  This is a library written for the Maxim MAX30105 Optical Smoke Detector
  It should also work with the MAX30102. However, the MAX30102 does not have a Green LED.
  These sensors use I2C to communicate, as well as a single (optional)
  interrupt line that is not currently supported in this driver.
  Written by Peter Jansen and Nathan Seidle (SparkFun)
  BSD license, all text above must be included in any redistribution.
 *****************************************************/

#include "max30105.h"

// I2C instance
static nrf_drv_twi_t* g_ptr_m_twi;
static twi_event_t* g_ptr_twi_event;

static uint8_t MAX30105_i2caddr;
static uint8_t MAX30105_activeLEDs; //Gets set during setup. Allows check() to calculate how many bytes to read from FIFO
static uint8_t MAX30105_revisionID;



//The MAX30105 stores up to 32 samples on the IC
//This is additional local storage to the microcontroller
//const int STORAGE_SIZE = 32; //Each uint32_t is 4 bytes so this
//is a 4*2*4 = 32 (+2) = 34 byte record ... make sure it fits on micro
//STORAGE_SIZE is a define

#define	STORAGE_SIZE 1

struct Record
{
    uint32_t red[STORAGE_SIZE];
    uint32_t IR[STORAGE_SIZE];
    //uint32_t green[STORAGE_SIZE];
    uint8_t head;
    uint8_t tail;
} sense; //This is our circular buffer of readings from the sensor


//
// Configuration
//

//Begin Interrupt configuration
uint8_t MAX30105_getINT1(void)
{
    return (MAX30105_read8(MAX30105_INTSTAT1));
}
uint8_t MAX30105_getINT2(void)
{
    return (MAX30105_read8(MAX30105_INTSTAT2));
}

void MAX30105_enableAFULL(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_A_FULL_MASK, MAX30105_INT_A_FULL_ENABLE);
}
void MAX30105_disableAFULL(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_A_FULL_MASK, MAX30105_INT_A_FULL_DISABLE);
}

void MAX30105_enableDATARDY(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_DATA_RDY_MASK, MAX30105_INT_DATA_RDY_ENABLE);
}
void MAX30105_disableDATARDY(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_DATA_RDY_MASK, MAX30105_INT_DATA_RDY_DISABLE);
}

void MAX30105_enableALCOVF(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_ALC_OVF_MASK, MAX30105_INT_ALC_OVF_ENABLE);
}
void MAX30105_disableALCOVF(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_ALC_OVF_MASK, MAX30105_INT_ALC_OVF_DISABLE);
}

void MAX30105_enablePROXINT(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_PROX_INT_MASK, MAX30105_INT_PROX_INT_ENABLE);
}
void MAX30105_disablePROXINT(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE1, MAX30105_INT_PROX_INT_MASK, MAX30105_INT_PROX_INT_DISABLE);
}

void MAX30105_enableDIETEMPRDY(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE2, MAX30105_INT_DIE_TEMP_RDY_MASK, MAX30105_INT_DIE_TEMP_RDY_ENABLE);
}
void MAX30105_disableDIETEMPRDY(void)
{
    MAX30105_bitMask(MAX30105_INTENABLE2, MAX30105_INT_DIE_TEMP_RDY_MASK, MAX30105_INT_DIE_TEMP_RDY_DISABLE);
}

//End Interrupt configuration

void MAX30105_softReset(void)
{
    uint32_t softReset_baseline;

    MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_RESET_MASK, MAX30105_RESET);

    //Poll for bit to clear, reset is then complete
    // Timeout after 100ms

    softReset_baseline = appHal_rtc_elapsed_ms();
    while ( appHal_rtc_elapsed_since_ms(softReset_baseline) < 100)
    {
        uint8_t response = MAX30105_read8(MAX30105_MODECONFIG);
        if ((response & MAX30105_RESET) == 0) break; //We're done!
        //Let's not over burden the I2C bus
        nrf_delay_ms(1);
    }

}

void MAX30105_shutDown(void)
{
    // Put IC into low power mode (datasheet pg. 19)
    // During shutdown the IC will continue to respond to I2C commands but will
    // not update with or take new readings (such as temperature)
    MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_SHUTDOWN_MASK, MAX30105_SHUTDOWN);
}

void MAX30105_wakeUp(void)
{
    // Pull IC out of low power mode (datasheet pg. 19)
    MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_SHUTDOWN_MASK, MAX30105_WAKEUP);
}

void MAX30105_setLEDMode(uint8_t mode)
{
    // Set which LEDs are used for sampling -- Red only, RED+IR only, or custom.
    // See datasheet, page 19
    MAX30105_bitMask(MAX30105_MODECONFIG, MAX30105_MODE_MASK, mode);
}

void MAX30105_setADCRange(uint8_t adcRange)
{
    // adcRange: one of MAX30105_ADCRANGE_2048, _4096, _8192, _16384
    MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_ADCRANGE_MASK, adcRange);
}

void MAX30105_setSampleRate(uint8_t sampleRate)
{
    // sampleRate: one of MAX30105_SAMPLERATE_50, _100, _200, _400, _800, _1000, _1600, _3200
    MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_SAMPLERATE_MASK, sampleRate);
}

void MAX30105_setPulseWidth(uint8_t pulseWidth)
{
    // pulseWidth: one of MAX30105_PULSEWIDTH_69, _188, _215, _411
    MAX30105_bitMask(MAX30105_PARTICLECONFIG, MAX30105_PULSEWIDTH_MASK, pulseWidth);
}

// NOTE: Amplitude values: 0x00 = 0mA, 0x7F = 25.4mA, 0xFF = 50mA (typical)
// See datasheet, page 21
void MAX30105_setPulseAmplitudeRed(uint8_t amplitude)
{
    MAX30105_write8(MAX30105_LED1_PULSEAMP, amplitude);
}

void MAX30105_setPulseAmplitudeIR(uint8_t amplitude)
{
    MAX30105_write8(MAX30105_LED2_PULSEAMP, amplitude);
}

/*
void MAX30105_setPulseAmplitudeGreen(uint8_t amplitude) {
  MAX30105_write8(MAX30105_LED3_PULSEAMP, amplitude);
}
*/

void MAX30105_setPulseAmplitudeProximity(uint8_t amplitude)
{
    MAX30105_write8(MAX30105_LED_PROX_AMP, amplitude);
}

void MAX30105_setProximityThreshold(uint8_t threshMSB)
{
    // Set the IR ADC count that will trigger the beginning of particle-sensing mode.
    // The threshMSB signifies only the 8 most significant-bits of the ADC count.
    // See datasheet, page 24.
    MAX30105_write8(MAX30105_PROXINTTHRESH, threshMSB);
}

//Given a slot number assign a thing to it
//Devices are SLOT_RED_LED or SLOT_RED_PILOT (proximity)
//Assigning a SLOT_RED_LED will pulse LED
//Assigning a SLOT_RED_PILOT will ??
void MAX30105_enableSlot(uint8_t slotNumber, uint8_t device)
{

//  uint8_t originalContents;

    switch (slotNumber)
    {
    case (1):
        MAX30105_bitMask(MAX30105_MULTILEDCONFIG1, MAX30105_SLOT1_MASK, device);
        break;
    case (2):
        MAX30105_bitMask(MAX30105_MULTILEDCONFIG1, MAX30105_SLOT2_MASK, device << 4);
        break;
    case (3):
        MAX30105_bitMask(MAX30105_MULTILEDCONFIG2, MAX30105_SLOT3_MASK, device);
        break;
    case (4):
        MAX30105_bitMask(MAX30105_MULTILEDCONFIG2, MAX30105_SLOT4_MASK, device << 4);
        break;
    default:
        //Shouldn't be here!
        break;
    }
}

//Clears all slot assignments
void MAX30105_disableSlots(void)
{
    MAX30105_write8(MAX30105_MULTILEDCONFIG1, 0);
    MAX30105_write8(MAX30105_MULTILEDCONFIG2, 0);
}

//
// FIFO Configuration
//

//Set sample average (Table 3, Page 18)
void MAX30105_setFIFOAverage(uint8_t numberOfSamples)
{
    MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_SAMPLEAVG_MASK, numberOfSamples);
}

//Resets all points to start in a known state
//Page 15 recommends clearing FIFO before beginning a read
void MAX30105_clearFIFO(void)
{
    MAX30105_write8(MAX30105_FIFOWRITEPTR, 0);
    MAX30105_write8(MAX30105_FIFOOVERFLOW, 0);
    MAX30105_write8(MAX30105_FIFOREADPTR, 0);
}

//Enable roll over if FIFO over flows
void MAX30105_enableFIFORollover(void)
{
    MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_ROLLOVER_MASK, MAX30105_ROLLOVER_ENABLE);
}

//Disable roll over if FIFO over flows
void MAX30105_disableFIFORollover(void)
{
    MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_ROLLOVER_MASK, MAX30105_ROLLOVER_DISABLE);
}

//Set number of samples to trigger the almost full interrupt (Page 18)
//Power on default is 32 samples
//Note it is reverse: 0x00 is 32 samples, 0x0F is 17 samples
void MAX30105_setFIFOAlmostFull(uint8_t numberOfSamples)
{
    MAX30105_bitMask(MAX30105_FIFOCONFIG, MAX30105_A_FULL_MASK, numberOfSamples);
}

//Read the FIFO Write Pointer
uint8_t MAX30105_getWritePointer(void)
{
    return (MAX30105_read8(MAX30105_FIFOWRITEPTR));
}

//Read the FIFO Read Pointer
uint8_t MAX30105_getReadPointer(void)
{
    return (MAX30105_read8(MAX30105_FIFOREADPTR));
}


// Die Temperature
// Returns temp in C
float MAX30105_readTemperature()
{
    int8_t tempInt = 0;
    uint8_t tempFrac = 0;
    uint32_t temperature_baseline;

    // Step 1: Config die temperature register to take 1 temperature sample
    MAX30105_write8(MAX30105_DIETEMPCONFIG, 0x01);

    // Poll for bit to clear, reading is then complete
    // Timeout after 100ms

    temperature_baseline = appHal_rtc_elapsed_ms();

    while ( appHal_rtc_elapsed_since_ms(temperature_baseline) < 100)
    {
        uint8_t response = MAX30105_read8(MAX30105_DIETEMPCONFIG);
        if ((response & 0x01) == 0) break; //Go ahead!
        //Let's not over burden the I2C bus
        nrf_delay_ms(1);
    }
    //TODO How do we want to fail? With what type of error?
    //? if(millis() - startTime >= 100) return(-999.0);

    // Step 2: Read die temperature register (integer)
    tempInt = MAX30105_read8(MAX30105_DIETEMPINT);
    tempFrac = MAX30105_read8(MAX30105_DIETEMPFRAC);

    // Step 3: Calculate temperature (datasheet pg. 23)
    return (float)tempInt + ((float)tempFrac * 0.0625);

}

// Returns die temp in F
float MAX30105_readTemperatureF()
{
    float temp = MAX30105_readTemperature();

    if (temp != -999.0) temp = temp * 1.8 + 32.0;

    return (temp);
}

// Set the PROX_INT_THRESHold
void MAX30105_setPROXINTTHRESH(uint8_t val)
{
    MAX30105_write8(MAX30105_PROXINTTHRESH, val);
}


//
// Device ID and Revision
//
uint8_t MAX30105_readPartID(void)
{
    return MAX30105_read8(MAX30105_PARTID);
}

void MAX30105_readRevisionID(void)
{
    MAX30105_revisionID = MAX30105_read8(MAX30105_REVISIONID);
}

uint8_t MAX30105_getRevisionID(void)
{
    return MAX30105_revisionID;
}


//Setup the sensor
//The MAX30105 has many settings. By default we select:
// Sample Average = 4
// Mode = MultiLED
// ADC Range = 16384 (62.5pA per LSB)
// Sample rate = 50
//Use the default setup if you are just getting started with the MAX30105 sensor
//MAX30105_setup(0x3F, 8, 2, 400, 411, 4096)
void MAX30105_setup(uint8_t powerLevel, uint8_t sampleAverage, uint8_t ledMode,
                    int sampleRate, int pulseWidth, int adcRange)
{
    MAX30105_softReset(); //Reset all configuration, threshold, and data registers to POR values

    //FIFO Configuration
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //The chip will average multiple samples of same type together if you wish
    if (sampleAverage == 1) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_1); //No averaging per FIFO record
    else if (sampleAverage == 2) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_2);
    else if (sampleAverage == 4) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_4);
    else if (sampleAverage == 8) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_8);
    else if (sampleAverage == 16) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_16);
    else if (sampleAverage == 32) MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_32);
    else MAX30105_setFIFOAverage(MAX30105_SAMPLEAVG_4);

    //setFIFOAlmostFull(2); //Set to 30 samples to trigger an 'Almost Full' interrupt
    MAX30105_enableFIFORollover(); //Allow FIFO to wrap/roll over
    // MAX30105_disableFIFORollover(); //**DON'T** Allow FIFO to wrap/roll over
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    //Mode Configuration
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //if (ledMode == 3) MAX30105_setLEDMode(MAX30105_MODE_MULTILED); //Watch all three LED channels
    if (ledMode == 2) MAX30105_setLEDMode(MAX30105_MODE_REDIRONLY); //Red and IR ... SPO2 MODE*
    else MAX30105_setLEDMode(MAX30105_MODE_REDONLY); //Red only
    MAX30105_activeLEDs = ledMode; //Used to control how many bytes to read from FIFO buffer
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    //Particle Sensing Configuration
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    if(adcRange < 4096) MAX30105_setADCRange(MAX30105_ADCRANGE_2048); //7.81pA per LSB
    else if(adcRange < 8192) MAX30105_setADCRange(MAX30105_ADCRANGE_4096); //15.63pA per LSB
    else if(adcRange < 16384) MAX30105_setADCRange(MAX30105_ADCRANGE_8192); //31.25pA per LSB
    else if(adcRange == 16384) MAX30105_setADCRange(MAX30105_ADCRANGE_16384); //62.5pA per LSB
    else MAX30105_setADCRange(MAX30105_ADCRANGE_2048);

    if (sampleRate < 100) MAX30105_setSampleRate(MAX30105_SAMPLERATE_50); //Take 50 samples per second
    else if (sampleRate < 200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_100);
    else if (sampleRate < 400) MAX30105_setSampleRate(MAX30105_SAMPLERATE_200);
    else if (sampleRate < 800) MAX30105_setSampleRate(MAX30105_SAMPLERATE_400);
    else if (sampleRate < 1000) MAX30105_setSampleRate(MAX30105_SAMPLERATE_800);
    else if (sampleRate < 1600) MAX30105_setSampleRate(MAX30105_SAMPLERATE_1000);
    else if (sampleRate < 3200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_1600);
    else if (sampleRate == 3200) MAX30105_setSampleRate(MAX30105_SAMPLERATE_3200);
    else MAX30105_setSampleRate(MAX30105_SAMPLERATE_50);

    //The longer the pulse width the longer range of detection you'll have
    //At 69us and 0.4mA it's about 2 inches
    //At 411us and 0.4mA it's about 6 inches
    if (pulseWidth < 118) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_69); //Page 26, Gets us 15 bit resolution
    else if (pulseWidth < 215) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_118); //16 bit resolution
    else if (pulseWidth < 411) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_215); //17 bit resolution
    else if (pulseWidth == 411) MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_411); //18 bit resolution
    else MAX30105_setPulseWidth(MAX30105_PULSEWIDTH_69);
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    //LED Pulse Amplitude Configuration
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //Default is 0x1F which gets us 6.4mA
    //powerLevel = 0x02, 0.4mA - Presence detection of ~4 inch
    //powerLevel = 0x1F, 6.4mA - Presence detection of ~8 inch
    //powerLevel = 0x7F, 25.4mA - Presence detection of ~8 inch
    //powerLevel = 0xFF, 50.0mA - Presence detection of ~12 inch

    MAX30105_setPulseAmplitudeRed(powerLevel-0x08);
    MAX30105_setPulseAmplitudeIR(powerLevel);
//  MAX30105_setPulseAmplitudeGreen(powerLevel);
    MAX30105_setPulseAmplitudeProximity(powerLevel);
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    //Multi-LED Mode Configuration, Enable the reading of the three LEDs
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    MAX30105_enableSlot(1, SLOT_RED_LED);
    if (ledMode > 1) MAX30105_enableSlot(2, SLOT_IR_LED);
    //if (ledMode > 2) MAX30105_enableSlot(3, SLOT_GREEN_LED);
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
    MAX30105_enableDATARDY();

    MAX30105_clearFIFO(); //Reset the FIFO before we begin checking the sensor
    if (!appHal_read(PIN_OXI_INT))
        MAX30105_read8(MAX30105_INTSTAT1);
}

//
// Data Collection
//

//Tell caller how many samples are available
uint8_t MAX30105_available(void)
{
    uint8_t numberOfSamples = sense.head - sense.tail;
    if (numberOfSamples < 0) numberOfSamples += STORAGE_SIZE;

    return (numberOfSamples);
}

//Report the most recent red value
uint32_t MAX30105_getRed(void)
{
    //Check the sensor for new data for 250ms
    if(MAX30105_safeCheck(250))
        return (sense.red[sense.head]);
    else
        return(0); //Sensor failed to find new data
}


//Report the most recent IR value
uint32_t MAX30105_getIR(void)
{
    //Check the sensor for new data for 250ms
    if(MAX30105_safeCheck(250))
        return (sense.IR[sense.head]);
    else
        return(0); //Sensor failed to find new data
}

////Report the most recent red value
//uint32_t MAX30105_getRedIR(int32_t *data)
//{
//    //Check the sensor for new data for 250ms
//    if(MAX30105_safeCheck(250)) {
//        data[0] = (int32_t) sense.red[sense.head];
//        data[1] = (int32_t) sense.IR[sense.head];
//        return(1UL); //Sensor failed to find new data
//    }
//    else {
//        return(0UL); //Sensor failed to find new data
//    }
//}
static int reEnter;

bool MAX30105_getRedIR(int32_t *data)
{
    uint8_t temp[ (sizeof(uint32_t)-1) * MAX30105_activeLEDs ];

    uint32_t redTempLong = 0;
    uint32_t irTempLong = 0;

    int numberOfSamples = 0;

    //Do we have new data?
    if (!appHal_read(PIN_OXI_INT) || reEnter)
    {
        redTempLong = irTempLong = 0;
        //Burst read 6 bytes - RED and IR
        MAX30105_read8_multi(MAX30105_FIFODATA, temp, sizeof(temp));
        //Convert array to long
        redTempLong = (temp[0]<<16) | (temp[1]<<8) | temp[2];
//        redTempLong &= 0x3FFFF; //Zero out all but 18 bits
        irTempLong = (temp[3]<<16) | (temp[4]<<8) | temp[5];
//        irTempLong &= 0x3FFFF;

        uint8_t readPointer = MAX30105_getReadPointer();
        uint8_t writePointer = MAX30105_getWritePointer();

        if (readPointer == writePointer) // we've read all data from the MAX30105 fifo
        {
            MAX30105_read8(MAX30105_INTSTAT1);
            reEnter = 0;
        }
        else // there is still data in the MAX30105 fifo
        {
            reEnter = 1;
        }
        data[0] = redTempLong;
        data[1] = irTempLong;

        return true;
    }
    else
        return false;
}

/*
//Report the most recent Green value
uint32_t MAX30105_getGreen(void)
{
  //Check the sensor for new data for 250ms
  if(MAX30105_safeCheck(250))
    return (sense.green[sense.head]);
  else
    return(0); //Sensor failed to find new data
}
*/

//Report the next Red value in the FIFO
uint32_t MAX30105_getFIFORed(void)
{
    return (sense.red[sense.tail]);
}

//Report the next IR value in the FIFO
uint32_t MAX30105_getFIFOIR(void)
{
    return (sense.IR[sense.tail]);
}

/*
//Report the next Green value in the FIFO
uint32_t MAX30105_getFIFOGreen(void)
{
  return (sense.green[sense.tail]);
}
*/

//Advance the tail
void MAX30105_nextSample(void)
{
    if(MAX30105_available()) //Only advance the tail if new data is available
    {
        sense.tail++;
        sense.tail %= STORAGE_SIZE; //Wrap condition
    }
}

// //Polls the sensor for new data
// //Call regularly
// //If new data is available, it updates the head and tail in the main struct
// //Returns number of new samples obtained
// uint16_t MAX30105_check(void)
// {
//     //Read register FIDO_DATA in (3-byte * number of active LED) chunks
//     //Until FIFO_RD_PTR = FIFO_WR_PTR
//     //Array of 4 bytes that we will convert into long
//     uint8_t temp[ (sizeof(uint32_t)-1) * MAX30105_activeLEDs ];

//     uint32_t redTempLong;
//     uint32_t irTempLong;

//     int numberOfSamples = 0;

//     //Do we have new data?
//     if (!appHal_read(PIN_OXI_INT))
//     {
//         uint8_t readPointer = MAX30105_getReadPointer();
//         uint8_t writePointer = MAX30105_getWritePointer();

//         if (readPointer == writePointer)
//             MAX30105_read8(MAX30105_INTSTAT1);
//         else
//         {
//             //Calculate the number of readings we need to get from sensor
//             numberOfSamples = writePointer - readPointer;
//             if (numberOfSamples < 0) numberOfSamples += 32; //Wrap condition

//             //We now have the number of readings, now calc bytes to read
//             //For this example we are just doing Red and IR (3 bytes each)
//             int bytesLeftToRead = numberOfSamples * MAX30105_activeLEDs * 3;


//             //We may need to read as many as 288 bytes so we read in blocks no larger than I2C_BUFFER_LENGTH
//             //I2C_BUFFER_LENGTH changes based on the platform. 64 bytes for SAMD21, 32 bytes for Uno.
//             //Wire.requestFrom() is limited to BUFFER_LENGTH which is 32 on the Uno
//             while (bytesLeftToRead > 0)
//             {
//                 int toGet = bytesLeftToRead;
//                 if (toGet > I2C_BUFFER_LENGTH)
//                 {
//                     //If toGet is 32 this is bad because we read 6 bytes (Red+IR * 3 = 6) at a time
//                     //32 % 6 = 2 left over. We don't want to request 32 bytes, we want to request 30.
//                     //32 % 9 (Red+IR+GREEN) = 5 left over. We want to request 27.

//                     //Trim toGet to be a multiple of the samples we need to read
//                     toGet = I2C_BUFFER_LENGTH - (I2C_BUFFER_LENGTH % (MAX30105_activeLEDs * 3));
//                 }

//                 bytesLeftToRead -= toGet;

//                 //Request toGet number of bytes from sensor
//                 while (toGet > 1) //use toGet > 1
//                 {
//                     sense.head++; //Advance the head of the storage struct
//                     sense.head %= STORAGE_SIZE; //Wrap condition

//                     if (MAX30105_activeLEDs == 2)
//                     {
//                         redTempLong = irTempLong = 0;
//                         //Burst read 6 bytes - RED and IR
//                         MAX30105_read8_multi(MAX30105_FIFODATA, temp, sizeof(temp));
//                         //Convert array to long
//                         redTempLong = (temp[0]<<16) | (temp[1]<<8) | temp[2];
//                         redTempLong &= 0x3FFFF; //Zero out all but 18 bits
//                         sense.red[sense.head] = redTempLong; //Store this reading into the sense array
//                         irTempLong = (temp[3]<<16) | (temp[4]<<8) | temp[5];
//                         irTempLong &= 0x3FFFF;
//                         sense.IR[sense.head] = irTempLong;
//                     }

//                     toGet -= MAX30105_activeLEDs * 3;
//                 } //while (toGet > 1)


//                 // now get the last sample!!!!!!
//                 sense.head++; //Advance the head of the storage struct
//                 sense.head %= STORAGE_SIZE; //Wrap condition


//                 if (MAX30105_activeLEDs == 2)
//                 {
//                     redTempLong = irTempLong = 0;
//                     //Burst read 6 bytes - RED and IR
//                     MAX30105_read8_multi(MAX30105_FIFODATA, temp, sizeof(temp));
//                     //Convert array to long
//                     redTempLong = (temp[0]<<16) | (temp[1]<<8) | temp[2];
//                     redTempLong &= 0x3FFFF; //Zero out all but 18 bits
//                     sense.red[sense.head] = redTempLong; //Store this reading into the sense array
//                     irTempLong = (temp[3]<<16) | (temp[4]<<8) | temp[5];
//                     irTempLong &= 0x3FFFF;
//                     sense.IR[sense.head] = irTempLong;
//                 }

//             } //End while (bytesLeftToRead > 0)

//         } //End readPtr != writePtr

//     } //!appHal_read(PIN_OXI_INT)

//     return (numberOfSamples); //Let the world know how much new data we found
// }


//Check for new data but give up after a certain amount of time
//Returns true if new data was found
//Returns false if new data was not found
uint8_t MAX30105_safeCheck(uint8_t maxTimeToCheck)
{
    /*
    systime_t safeCheck_baseline;

    safeCheck_baseline = appHal_rtc_elapsed_ms();
    while(1)
    {
    if(MAX30105_check() >= 1) //We found new data!
      return(1);

    if(appHal_rtc_elapsed_since_ms(safeCheck_baseline) > maxTimeToCheck) return(0);
    //_delay_ms(1);
    chThdSleepMilliseconds(2);
    }
    */
    if(MAX30105_check() >= 1)  //We found new data!
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

//Given a register, read it, mask it, and then set the thing
void MAX30105_bitMask(uint8_t reg, uint8_t mask, uint8_t thing)
{
    // Grab current register context
    uint8_t originalContents = MAX30105_read8(reg);

    // Zero-out the portions of the register we're interested in
    originalContents = originalContents & mask;

    // Change contents
    MAX30105_write8(reg, originalContents | thing);
}

//
// Low-level I2C Communication
//

uint8_t MAX30105_read8(uint8_t reg)
{
    uint8_t data;
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx(g_ptr_m_twi, MAX30105_ADDRESS, &reg, 1, true);
    wait_xfer();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_rx(g_ptr_m_twi, MAX30105_ADDRESS, &data, sizeof(data));
    wait_xfer();
    APP_ERROR_CHECK(err_code);

    return data;
}

void MAX30105_write8(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};
    ret_code_t err_code = nrf_drv_twi_tx(
                              g_ptr_m_twi, MAX30105_ADDRESS, data, sizeof(data), false);
    wait_xfer();
    APP_ERROR_CHECK(err_code);
}

void MAX30105_read8_multi(uint8_t reg, uint8_t* dataDest, uint8_t dataSize)
{
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx(g_ptr_m_twi, MAX30105_ADDRESS, &reg, 1, true);
    wait_xfer();
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_twi_rx(g_ptr_m_twi, MAX30105_ADDRESS, dataDest, dataSize);
    wait_xfer();
    APP_ERROR_CHECK(err_code);
}

void wait_xfer(void)
{
    while (g_ptr_twi_event->xfer_done == false);
    g_ptr_twi_event->xfer_done = false;
}

int MAX30105_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event)
{
    g_ptr_m_twi = ptr_twi;
    g_ptr_twi_event = ptr_twi_event;

    // check if the IC is responding
    if ((!MAX30105_readPartID()) == MAX_30105_EXPECTEDPARTID)
        return 0;

    // Populate revision ID
    MAX30105_readRevisionID();

    return 1;
}