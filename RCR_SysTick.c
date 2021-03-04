// RCR_SysTick.c
// Compatible with MSP432
// Abhi Kallur


// Provide functions for SysTick that can do
// a fixed loop delay in microseconds. This will
// mainly be used for throughput/performance measurements
// with SysTick registers.


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

#include <stdint.h>
#include "msp.h"


// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void) {
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
}

// Time delay using busy wait.
// assumes 48 MHz bus clock
//void SysTick_Wait10ms(uint32_t delay) {
//  uint32_t i;
//  for(i=0; i<delay; i++){
//    SysTick_Wait(480000);  // wait 10ms (assumes 48 MHz clock)
//  }
//}


// Time delay using SysTick
void SysTick_Wait1us(uint32_t delay, uint32_t clock_in_usec) {       // modify for 24 bit rollover
    SysTick->LOAD = ((clock_in_usec * delay) - 130) & 0x00FFFFFF;       // subtract 130 overhead cycles
    SysTick->VAL = 0;

    while((SysTick->CTRL & 0x00010000) == 0) {}
}
