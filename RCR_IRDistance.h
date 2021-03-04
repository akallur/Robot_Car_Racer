// RCR_IRDistance.h
// Compatible with MSP432
// Abhi Kallur

// Provides distance calculations for a Sharp GP2Y0A21YK0F
// sensor, along with functions for a software Low
// Pass Filter used on the raw ADC data


 /*Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

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


#ifndef RCR_IRDISTANCE_H_
#define RCR_IRDISTANCE_H_

#define RIGHT  0
#define CENTER 1
#define LEFT   2


///**
// * Convert ADC sample into distance for the GP2Y0A21YK0F
// * infrared distance sensor.  Conversion uses a calibration formula
// * R = (1/m)/(ADCval + (b/m)) - k
// */

uint32_t ConvertDist(uint32_t ADCval);

void LowPassFilter_Init(uint32_t num_channels,uint32_t size,uint32_t* init_data);

uint32_t LowPassFilter(uint32_t chnl,uint32_t data);

#endif /* RCR_IRDISTANCE_H_ */
