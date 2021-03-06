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
* to deal in the Software without restriction, including without limitation
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

#ifndef _HEARTRATE_H_
#define _HEARTRATE_H_

#include <stdint.h>
#include "max30105.h"
#include "appHal_rtc.h"
#include "appHal_gpio.h"
#include "SEGGER_RTT.h"

#define FIAR_PARAM 6
#define FILT_AV 32 //change also in heartrate.h
#define LOG_2_FILT_AV 5

struct HR_algo_state {
    uint8_t maybe_peak;
    uint8_t maybe_valley;
    uint8_t peak_detect;
    uint8_t valley_detect;
    uint8_t fiar; //stands for five-in-a-row
    int32_t valley_min;
    int32_t peak_max;
    uint8_t offset_DC;
    uint8_t offset_filt;
    int32_t xbuf_DC[32];//FILT_AV
    int32_t abuf_DC[32];
    int32_t cbuf[32];
};

void heart_rate_init (nrf_drv_twi_t* ptr_twi, twi_event_t* ptr_twi_event);
uint8_t heart_rate_process(uint8_t* heartRate);
uint8_t check_for_beat(int32_t *output_buffer, int32_t *avg_estimate, int32_t sample, struct HR_algo_state* HR_state);
int HR_sleep(void);
int HR_wake_up(void);

#endif