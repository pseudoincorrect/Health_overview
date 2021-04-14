/*
 Optical Heart Rate Detection (PBA Algorithm)
 By: Nathan Seidle
 SparkFun Electronics
 Date: October 2nd, 2016

 Given a series of IR samples from the MAX30105 we discern when a heart beat is occurring
 Let's have a brief chat about what this code does. We're going to try to detect
 heart-rate optically. This is tricky and prone to give false readings. We really don't
 want to get anyone hurt so use this code only as an example of how to process optical
 data. Build fun stuff with our MAX30105 breakout board but don't use it for actual
 medical diagnosis.
 Excellent background on optical heart rate detection:
 http://www.ti.com/lit/an/slaa655/slaa655.pdf
 Good reading:
 http://www.techforfuture.nl/fjc_documents/mitrabaratchi-measuringheartratewithopticalsensor.pdf
 https://fruct.org/publications/fruct13/files/Lau.pdf
 This is an implementation of Maxim's PBA (Penpheral Beat Amplitude) algorithm. It's been
 converted to work within the Arduino framework.
*/

/* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
*/

#include "heartRate.h"


#define POWER_LEVEL 0x3F
//1.5Hz
//static const int32_t FIRCoeffs[12] = {172L, 321L, 579L, 927L, 1360L, 1858L, 2390L, 2916L, 3391L, 3768L, 4012L, 4096L};//the original

//2Hz
static const int32_t FIRCoeffs[12] = {-111L, -83L, -10L, 202L, 603L, 1212L, 2009L, 2923L, 3842L, 4632L, 5167L, 5356L};//the original

//3Hz
//static const int32_t FIRCoeffs[12] = {-150L, -265L, -376L, -360L, -96L, 515L, 1502L, 2794L, 4210L, 5502L, 6409L, 6735L};//higher bw

/********************************************************************************
Heartbeat setup
********************************************************************************/
int32_t Red_LED_output[4] = {0L};
int32_t IR_LED_output[4] = {0L};
int32_t Red_dc_avg = 0L;
int32_t IR_dc_avg = 0L;
uint8_t Red_result = 0;
uint8_t IR_result = 0;
int32_t LED_data_vector[2] = {0L};

int spo_acq = 0;

uint8_t HB_ON = 0;//HB beating display
uint8_t HB_locked = 0;
uint32_t ms_current_beat = 0UL;
uint32_t ms_old_beat = 0UL;
uint32_t ms_elapsed_since_last = 0UL;
int32_t ms_elapsed_since_last_right_now = 0UL;
uint32_t blanking_time = 0UL;

struct HR_algo_state Red_HR_state;
struct HR_algo_state IR_HR_state;

static uint32_t time_now;
static uint32_t HB_baseline;
static int32_t current_HB_average         = 85L;
static int32_t current_HB_estimate        = 0L;
static int32_t current_HB_average_last    = 0L;
static int32_t unlock_test                = 0L;
static int32_t coeff                      = 5L;
static int32_t attack_max                 = 35L;

static int32_t SPO2_circular_buffer[7]    = {100L,100L,100L,100L,100L,100L,100L};
static int32_t SPO2_sum                   = 0L;
static uint8_t SPO2_circular_buffer_index = 0;
static uint8_t SPO2_index                 = 0;
static int32_t SPO2                       = 0L;
static int32_t R1                         = 0L;
static int32_t R2                         = 0L;
static int32_t R                          = 0L;

typedef struct SPO2_state
{
    uint32_t baseline;
    int32_t spread;
    uint8_t status;
} SPO2_state_t;

static SPO2_state_t Red_SPO2_state;
static SPO2_state_t IR_SPO2_state;


void heart_rate_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event)
{
    // initialize the Red SPO2 state variable
    Red_SPO2_state.baseline = 0;
    Red_SPO2_state.spread   = 0;
    Red_SPO2_state.status   = 0;

    // initialize the IR SPO2 state variable
    IR_SPO2_state.baseline  = 0;
    IR_SPO2_state.spread    = 0;
    IR_SPO2_state.status    = 0;

    // initialize the Red HR state variable
    Red_HR_state.maybe_peak    = 0;
    Red_HR_state.maybe_valley  = 0;
    Red_HR_state.peak_detect   = 0;
    Red_HR_state.valley_detect = 0;
    Red_HR_state.fiar          = 0; //stands for five-in-a-row
    Red_HR_state.valley_min    = 0L;
    Red_HR_state.peak_max      = 0L;
    Red_HR_state.offset_DC     = 0;
    Red_HR_state.offset_filt   = 0;

    // initialize the HR state variable
    IR_HR_state.maybe_peak    = 0;
    IR_HR_state.maybe_valley  = 0;
    IR_HR_state.peak_detect   = 0;
    IR_HR_state.valley_detect = 0;
    IR_HR_state.fiar          = 0; //stands for five-in-a-row
    IR_HR_state.valley_min    = 0L;
    IR_HR_state.peak_max      = 0L;
    IR_HR_state.offset_DC     = 0;
    IR_HR_state.offset_filt   = 0;

    for(int i = 0; i < 32; i++)
    {
        if (i < 4)
        {
            Red_HR_state.xbuf_DC[i] = 0L;
            Red_HR_state.abuf_DC[i] = 0L;
        }
        Red_HR_state.cbuf[i]= 0L;
    }

    for(int i = 0; i < 32; i++)
    {
        if (i < 4)
        {
            IR_HR_state.xbuf_DC[i] = 0L;
            IR_HR_state.abuf_DC[i] = 0L;
        }
        IR_HR_state.cbuf[i]= 0L;
    }

    if (!MAX30105_init(ptr_twi, ptr_twi_event))
    {
        SEGGER_RTT_printf(0, "cannot communicate with max30105\n");
    }
    nrf_delay_ms(50);

    // powerLevel  sampleAverage  ledMode  sampleRate  pulseWidth  adcRange
    MAX30105_setup(POWER_LEVEL, 8, 2, 400, 411, 4096);  // top wrist
//    MAX30105_setup(0x1F, 8, 2, 50, 411, 16384);   // finger tip

    nrf_delay_ms(50);

    HB_baseline = appHal_rtc_elapsed_ms();
}

uint8_t heart_rate_process(uint8_t* heartRate)
{
    static int i;
    // get current heartbeat
    int ret = 0;

    bool is_new_data = MAX30105_getRedIR(LED_data_vector);
    // if (is_new_data)
    //     SEGGER_RTT_printf(0, "%d\n", LED_data_vector[0]);

    if (is_new_data)   //new data
    {
        IR_result = check_for_beat(IR_LED_output, &IR_dc_avg, LED_data_vector[1], &IR_HR_state);//always
        
        SEGGER_RTT_printf(0, "%d\n\r", LED_data_vector[1]);
        // if (spo_acq)   // in SPO2 mode, need red
        // {
        //     Red_result = check_for_beat(Red_LED_output, &Red_dc_avg, LED_data_vector[0], &Red_HR_state);
        // }

        if (IR_result == 1)   //found peak, happens rarely
        {
            ms_current_beat = appHal_rtc_elapsed_since_ms(HB_baseline);

            //need to be careful here if it is a double, we throw it out ... get times right!!!
            ms_elapsed_since_last = ms_current_beat - ms_old_beat;

            ms_old_beat = ms_current_beat;
            ret = 1;
            if (ms_elapsed_since_last > blanking_time)
            {
                current_HB_estimate = 60000L/(int32_t)ms_elapsed_since_last;
                if ((current_HB_estimate < 20L) || (current_HB_estimate > 230L))   //then we have a bad value
                {;}
                else
                {
                    //HEART BEAT FILTER & test whether its "locked"
                    //ADJUST HB with a partial step to current value
                    current_HB_average_last = current_HB_average;
                    HB_ON = 1;
                    if (    (current_HB_estimate > (current_HB_average>>2) )
                         && (current_HB_estimate < (current_HB_average<<2) ))
                    {
                        coeff =  (90L*60L*3L)/(8L*current_HB_average);
                        if (coeff > attack_max) coeff = attack_max;
                    }
                    else coeff = 5L;
                    if (current_HB_average > current_HB_estimate)
                    {
                        //K --;
                        unlock_test = current_HB_average - current_HB_estimate;
                        current_HB_average -= (coeff*(current_HB_average-current_HB_estimate))/100;
                    }
                    else
                    {
                        //K++;
                        unlock_test = current_HB_estimate - current_HB_average;
                        current_HB_average += (coeff*(current_HB_estimate-current_HB_average))/100;
                    }
                    if (current_HB_average <=0)
                        current_HB_average = current_HB_average_last;

                    blanking_time = 30000L/(uint32_t)current_HB_average;

                    *heartRate = current_HB_average;

                    if ((100L*unlock_test) < (10L*current_HB_average_last))
                    {
                        HB_locked = 1;
                    }
                    else
                    {
                        HB_locked = 0;
                    }
                }//HB filter

                // if (HB_locked ==1) { // in SPO2 mode, validated peak, grab IR data
                //     IR_SPO2_state.baseline = IR_dc_avg;
                //     IR_SPO2_state.spread = IR_HR_state.peak_max - IR_HR_state.valley_min;
                //     IR_SPO2_state.status = 1;
                // }
            }//ms_elapsed > blanking_time
        }//IR_result==1, found peak

        else  //result = 0, didn't find peak, finish HB visualization
        {
            ms_elapsed_since_last_right_now = appHal_rtc_elapsed_since_ms(HB_baseline) - ms_old_beat;
        }

        //SPO2, in a good HB regime, IR SPO2 is good, Red found
        // if ((IR_SPO2_state.status == 1)&&(Red_result == 1)) {
        //     Red_SPO2_state.baseline = Red_dc_avg;
        //     Red_SPO2_state.spread = Red_HR_state.peak_max - Red_HR_state.valley_min;
        //     Red_SPO2_state.status = 1;
        //     if (Red_SPO2_state.status == 1) {//calculate SPO2
        //         //calculate SPO2
        //         R1 = (100L*Red_SPO2_state.spread)/IR_SPO2_state.spread;
        //         R2 = (100L*IR_SPO2_state.baseline)/Red_SPO2_state.baseline;
        //         R = (R1*R2)/10L;
        //         if ((R>300L) && (R<2300L)) {
        //             //SPO2 = 106900L - ((39L*R)/10L) - ((156L*R*R)/10000L);
        //             SPO2 = 110000L - 25L*R; //simpler to start
        //             SPO2 /= 1000L;
        //             //fill and average circular buffer
        //             SPO2_circular_buffer[SPO2_circular_buffer_index] = SPO2;
        //             SPO2_circular_buffer_index++;
        //             SPO2_circular_buffer_index %= 7; //Wrap condition
        //         }
        //         SPO2_sum = 0L;
        //         for(SPO2_index = 0; SPO2_index < 7; SPO2_index++) {
        //             SPO2_sum += SPO2_circular_buffer[SPO2_index];
        //         }
        //         SPO2_sum /= 7L;
        //         //reset all variables
        //         IR_SPO2_state.status = 0;
        //         Red_SPO2_state.status = 0;
        //     }
        // }//red detected

    }   //if (p2 > 0) - new data
    
    // appHal_write(PIN_LEDG,  1);
    return ret;
}



/**********************************************
state parameters for detection algorithm
**********************************************/

//  Heart Rate Monitor functions takes a sample value and the sample number
//  Returns true if a beat is detected
//  A running average of four samples is recommended for display on the screen.
uint8_t check_for_beat(int32_t *output_buffer, int32_t *avg_estimate, int32_t sample,
                     struct HR_algo_state* HR_state)
{
    uint8_t beatDetected = 0;
    uint8_t i = 0;
    int32_t z = 0;
    *avg_estimate = 0L;

    //  Save current state
    output_buffer[2] = output_buffer[1];
    output_buffer[1] = output_buffer[0];

    HR_state->xbuf_DC[HR_state->offset_DC] = sample;

    HR_state->abuf_DC[HR_state->offset_DC] = 0L;

    /*****  Strip out the DC *********/
    for (i = 0 ; i < FILT_AV ; i++)
    {
        // sum all buf_DC
        HR_state->abuf_DC[HR_state->offset_DC] += HR_state->xbuf_DC[i];
    }
    // average it 
    HR_state->abuf_DC[HR_state->offset_DC] = (HR_state->abuf_DC[HR_state->offset_DC])>>LOG_2_FILT_AV;

    for (i = 0 ; i < FILT_AV ; i++)
    {
        *avg_estimate += HR_state->abuf_DC[i];
    }
    *avg_estimate = *avg_estimate>>LOG_2_FILT_AV;

    output_buffer[0] = (*avg_estimate - HR_state->xbuf_DC[(HR_state->offset_DC+1)%FILT_AV])<<2 ;
    HR_state->offset_DC++;
    HR_state->offset_DC %= FILT_AV; //Wrap condition
    /*****  Strip out the DC *********/

    /*****  Low Pass FIR Filter *********/
    HR_state->cbuf[HR_state->offset_filt] = output_buffer[0];
    z = FIRCoeffs[11] * HR_state->cbuf[(HR_state->offset_filt - 11) & 0x1F];

    for (uint8_t i = 0 ; i < 11 ; i++)
    {
        z +=
            FIRCoeffs[i]
            *
            (
                HR_state->cbuf[(HR_state->offset_filt - i) & 0x1F]
                +
                HR_state->cbuf[(HR_state->offset_filt - 22 + i) & 0x1F]
            );
    }

    HR_state->offset_filt++;
    HR_state->offset_filt %= 32; //Wrap condition

    output_buffer[0] = (z>>8);//the original
    output_buffer[0] *= -1;//the original
    /*****  Low Pass FIR Filter *********/

    /*****  Actual Algorithm *********/
    if ((output_buffer[2]  == output_buffer[1]) && (output_buffer[1] < output_buffer[0]))
    {
        if ( HR_state->maybe_valley==1)
        {
            HR_state->valley_detect = 1;
            HR_state->valley_min = output_buffer[1];
            HR_state->maybe_valley = 0;
        }
        if ( HR_state->maybe_peak==1)
        {
            HR_state->maybe_peak = 0;
        }
    }

    //dot-flat-down
    if ((output_buffer[2]  == output_buffer[1]) && (output_buffer[1] > output_buffer[0]))
    {
        if ( HR_state->maybe_valley ==1)
        {
            HR_state->maybe_valley = 0;
        }
        if (( HR_state->maybe_peak==1)&&(output_buffer[1] >0))
        {
            HR_state->maybe_peak = 0;
            HR_state->peak_detect = 1;
            HR_state->peak_max = output_buffer[1];
        }
    }

    //dot-up-flat
    if ((output_buffer[2] < output_buffer[1]) && (output_buffer[1] == output_buffer[0]))
    {
        HR_state->maybe_peak = 1;
    }

    //dot-up-down
    if ((output_buffer[2]  < output_buffer[1]) && (output_buffer[1] > output_buffer[0]) && (output_buffer[1] >0))
    {
        HR_state->peak_detect = 1;
        HR_state->peak_max = output_buffer[1];
    }

    //dot-down-up
    if ((output_buffer[2]  > output_buffer[1]) && (output_buffer[1] < output_buffer[0]))
    {
        HR_state->valley_detect = 1;
        HR_state->valley_min = output_buffer[1];
    }

    //dot-down-flat
    if ((output_buffer[2]  > output_buffer[1]) && (output_buffer[1] == output_buffer[0]))
    {
        HR_state->maybe_valley = 1;
    }

    //  special: detect fiar (five-in-a-row going up)
    if ( HR_state->fiar < FIAR_PARAM)
    {
        if (output_buffer[1] <= output_buffer[0])
        {
            HR_state->fiar++;
        } // detect fiar
        else
        {
            if (HR_state->fiar >0)
            {
                HR_state->fiar = 0;
            }
        }
    }

    //  if detected peak
    if ( HR_state->peak_detect ==1)
    {
        // check whether its a valid beat
        if((HR_state->valley_detect ==1)
                && (  HR_state->fiar == FIAR_PARAM)
                && (( HR_state->peak_max -  HR_state->valley_min) > 75L)
                && (( HR_state->peak_max -  HR_state->valley_min) < 2500000L))
        {
            //Heart beat!!!
            beatDetected = 1;
        }
        //peak was detected, so no matter what, reset everything
        HR_state->maybe_peak = 0;
        HR_state->maybe_valley = 0;
        HR_state->peak_detect = 0;
        HR_state->valley_detect = 0;
        HR_state->fiar = 0;
        //HR_state->valley_min = 0;
        //HR_state->peak_max = 0;
    }//detect peak

    return(beatDetected);
}


int HR_sleep(void)
{
    MAX30105_shutDown();
    MAX30105_setPulseAmplitudeRed(0);
    MAX30105_setPulseAmplitudeIR(0);
    return 0;
}

int HR_wake_up(void)
{
    MAX30105_wakeUp();
    MAX30105_setPulseAmplitudeRed(POWER_LEVEL-0x08);
    MAX30105_setPulseAmplitudeIR(POWER_LEVEL);
    MAX30105_clearFIFO(); //Reset the FIFO before we begin checking the sensor
    MAX30105_read8(MAX30105_INTSTAT1);
    return 0;
}