// RCR_TimerA1.c
// Compatible with MSP432
// Abhi Kallur

// Initializes Timer A1 interrupts to call
// a task periodically. This functions as a
// general purpose timer that can be set to
// any function and is currently used for
// periodic ADC sampling.


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

static void (*TimerA1Task)(void);   // user function

// ***************** TimerA1_Init ****************
// Activate Timer A1 interrupt to run user task periodically.
// Currently used for ADC14, but can be repurposed.
// The clock is set to 12 MHz / 64 = 0.1875 MHz
// Period must fit within 16 bits and calls task function.
void TimerA1_Init(void(*task)(void), uint16_t period) {
    TimerA1Task = task;

    TA1CTL  &= ~0x0030;     //halt timer
    TA1CTL   = 0x02C0;    //halt timer, smclk_12MHz, divide by 8
    TA1EX0   = 0x0007;    //set for another divide by 8

    TA1CCTL0 = 0x0010;      //compare mode, out mode is not used, arm interrupt
    TA1CCR0  = period;      //load in cycles

    NVIC->IP[10]   = 0x20;          //priority 1
    NVIC->ISER[0] |= 0x00000400;    //enable TA1_0(irq 10) interrupt

    TA1CTL  |= 0x0014;     //reset counter and set for up mode
}


// ------------TimerA1_Stop------------
// Deactivate the interrupt running a user task periodically.
void TimerA1_Stop(void) {
    NVIC->ICER[0] |= 0x00000400;    //disable TA1_0(irq 10) interrupt
}


void TA1_0_IRQHandler(void) {
    TA1CCTL0 &= ~0x0001;    //clear interrupt flag
    (*TimerA1Task)();
}
