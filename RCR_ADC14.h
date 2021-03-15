// RCR_ADC14.h
// Compatible with MSP432
// Abhi Kallur

// Provides driver functions for ADC
// sampling to collect data for IR sensors.
// Currently set up for channels 14, 16, and
// 17 to get raw data as requested by software.


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


#ifndef RCR_ADC14_H_
#define RCR_ADC14_H_



#define ANALOG_CHNLS 3      //number of analog sensors on system

/*
 Hardware connections
 ---------------------------------------------------------
 P9.0 = analog channel 17, right sensor
 P6.1 = analog channel 14, center sensor
 P9.1 = analog channel 16, left sensor
*/


/*
  ADC0_Init_Ch17_14_16
  ----------------------------------------------------------------------
  Initialize 14-bit ADC0 in software-triggered mode to take measurements
  when the associated function is called. The sequence of measurements
  starts at channel 14, then channel 17, and ends at channel 16. Interrupts
  are disabled and Timer1A is used to time the samples. The 12 MHz SMCLK
  is used and sampling is set to 96 cycles and converting takes 16 cycles.
  The 3.3V analog supply is used as reference.

  Parameters:   none
  Return value: none
*/
void ADC0_Init_Ch17_14_16(void);



/*
  ADC_In17_14_16
  ----------------------------------------------------------------------
  Trigger a single ADC measurement on A17, A14, and
  A16 each, wait for them to complete, and put the results in
  the pointers given. The pointers are 32-bit integers
  because the ADC registers are 32 bits. In this case, bits
  31-16 are undefined and bits 15-14 are zero.
  Busy-wait synchronization used, ADC input voltage range is
  0 to 3.3V, and assumes ADC0_Init_Ch17_14_16() has been called
  and ADC is 14-bit precision.

  Parameters:   1) address to store ch 17/right sample
                2) address to store ch 14/center sample
                3) address to store ch 16/left sample
  Return value: none
*/
void ADC_In17_14_16(uint32_t *ch17, uint32_t *ch14, uint32_t *ch16);


#endif /* RCR_ADC14_H_ */
