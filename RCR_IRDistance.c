// RCR_IRDistance.c
// Compatible with MSP432
// Abhi Kallur

// Provides distance calculations for a Sharp GP2Y0A21YK0F
// sensor, along with functions for a software Low
// Pass Filter used on the raw ADC data



/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// 5V connected to all three IR sensors
// ground connected to all three IR sensors
// P9.0 (analog ch 17) connected to right sensor signal line
// P6.1 (analog ch 14) connected to center sensor signal line
// P9.1 (analog ch 16) connected to left sensor signal line


#include <stdint.h>
#include <stdlib.h>
//#include "ADC14.h"
#include "msp.h"

#define MAX_ANLG_CHNLS 32         //limit for MSP432 mcu

#define CAL_CONST      0.91       // k value
#define INV_SLOPE      126716.5   // 1/m value
#define INTERCEPT      575.6511   // b/m value
#define CM_TO_MM       10


struct Analog_Chnl
{
    uint32_t* data_buff;
    uint32_t  rolling_sum;
    uint32_t  buff_cnt;
};

typedef struct Analog_Chnl analog_chnl;

static analog_chnl* analog_sensors;     //array of all analog sensors/channels
static uint32_t     filter_size;        //standard filter size for all sensors(may make this dynamic if I add different sensors)

///**
// * Convert ADC sample into distance for the GP2Y0A21YK0F
// * infrared distance sensor.  Conversion uses a calibration formula
// * R = (1/m)/(ADCval + (b/m)) - k
// */
uint32_t ConvertDist(uint32_t ADCval) {        // returns distance in mm
    return ((INV_SLOPE/(ADCval + INTERCEPT)) - CAL_CONST)*CM_TO_MM;
}


void LowPassFilter_Init(uint32_t num_channels,uint32_t size,uint32_t* init_data) {
    int x,y;
    if(num_channels > MAX_ANLG_CHNLS) num_channels = MAX_ANLG_CHNLS;
    filter_size = size;

    analog_sensors = (analog_chnl*)malloc(num_channels*sizeof(analog_chnl));     //create buffer for all channels of any size
    for(x = 0; x < num_channels; x++)                                           //if I want more distance sensors, I can easily add more channels
    {
        analog_sensors[x].data_buff   = (uint32_t *)malloc(filter_size*sizeof(uint32_t));
        analog_sensors[x].rolling_sum = 0;
        analog_sensors[x].buff_cnt    = 0;
        for(y = 0; y < filter_size; y++)                                          //prefill with current sample
        {
            analog_sensors[x].data_buff[y] = init_data[x];
            analog_sensors[x].rolling_sum += analog_sensors[x].data_buff[y];      //calculate sum in advance
        }
    }
}

uint32_t LowPassFilter(uint32_t chnl,uint32_t data) {
    analog_chnl* current = &analog_sensors[chnl];                   //store current channels data
    current->rolling_sum += (2*data);                               //add 2 copies of latest sample, subtract 2 oldest samples
    current->rolling_sum -= current->data_buff[current->buff_cnt];
    current->rolling_sum -= current->data_buff[((current->buff_cnt+1)%filter_size)];
    current->data_buff[current->buff_cnt] = data;                   //update buffer and count and wrap if necessary
    current->data_buff[((current->buff_cnt+1)%filter_size)] = data;
    current->buff_cnt = (current->buff_cnt+2)%filter_size;
    return current->rolling_sum/filter_size;
}
