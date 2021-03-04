// RCR_TimerA0.c
// Compatible with MSP432
// Abhi Kallur

// Initializes Timer 0A module for use with
// motors. Timer 0A pins are used for PWM to
// control the speed of the motors. No
// interrupts needed, as the pins are set up
// to be controlled by the hardware.

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
#include "RCR_TimerA0.h"

// P2.7 (TimerA0 sub 4) connected to Left motor PWM
// P2.6 (TimerA0 sub 3) connected to Right motor PWM

//void (*TimerA0Task)(void);   // user function


/**
 * Initialize Timer A0 to run hardware task periodically.
 * Clock is set to 12 MHz / 8 = 1.5 MHz.
 * Will run PWM on output pins for the motors,
 * based on parameters sent for duty cycles.
 */
void TimerA0_Init(uint16_t period, uint16_t dutyLeft, uint16_t dutyRight) {
    if(dutyLeft >= period || dutyRight >= period)
    {
        dutyLeft = dutyRight = 0;
    }

    P2->SEL0  |=  0xC0;
    P2->SEL1  &= ~0xC0;
    P2->DIR   |=  0xC0;

    TA0CTL &= ~0x0030;
    TA0CTL =   0x02C0;    //ctl: halt timer, SMCLK, divide by 8
    TA0EX0 =   0x0000;    //ex: set for divide by 1

    TA0CCTL0 = 0x0080;   //cctl's: compare mode, outmode = reset/set
    TA0CCTL3 = 0x00E0;
    TA0CCTL4 = 0x00E0;
    TA0CCR0  = period;        //ccr's: load in cycles
    TA0CCR3  = dutyRight;
    TA0CCR4  = dutyLeft;

    TA0CTL  |=  0x0014;          //ctl: set for up mode, set clear bit
}


bool SetDuty_Right(uint16_t num_cycles) {
    if(num_cycles >= TA0CCR0) return false;
    TA0CCR3 = num_cycles;
    return true;
}

bool SetDuty_Left(uint16_t num_cycles) {
    if(num_cycles >= TA0CCR0) return false;
    TA0CCR4 = num_cycles;
    return true;
}

//void TA0_0_IRQHandler(void) {
//  TA0CCTL0 &= ~0x0001;        // clear interrupt flag
//  (*TimerA0Task)();
//}


