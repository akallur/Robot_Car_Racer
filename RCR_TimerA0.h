// RCR_TimerA0.h
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

#ifndef RCR_TIMERA0_H__
#define RCR_TIMERA0_H__


#include <stdbool.h>

/*
 Hardware connections
 ---------------------------------------------------------
 P2.7 (TimerA0 sub 4) connected to Left motor PWM
 P2.6 (TimerA0 sub 3) connected to Right motor PWM
*/


/*
  TimerA0_Init
  ----------------------------------------------------------------------
  This will initialize Timer A0 to run hardware task periodically. This
  timer functions as a PWM output to both motors, done through output
  pins. The SMCLK(12 MHz) is used and divided by 8 to run the timer at
  1.5 MHz. The period and duty cycles are selected based on the device's
  time constant and desired speed/intensity. It uses compare mode to set/reset the
  pins if the counter matches the selected duty cycle. The counter is set
  to count in the up direction. No interrupts needed as PWM output is
  handled in hardware.

  The brushed DC motors being used have a time constant of 100 ms. Our period
  is set to 10 ms(15000 clk cycles) for now.

  Parameters:   1) time(in clk cycles) to calculate duty cycles from,
                       must fit within 16 bits
                2) speed(in clk cycles) of left motor as percentage of period(dutyLeft/period),
                       must be < period-1
                3) speed(in clk cycles) of right motor as percentage of period(dutyRight/period),
                       must be < period-1
  Return value: none
*/
void TimerA0_Init(uint16_t period, uint16_t dutyLeft, uint16_t dutyRight);

/*
  SetDuty_Right
  ----------------------------------------------------------------------
  Changes the speed of the motor to selected duty cycle.

  Parameters:   1) speed(in clk cycles) of right motor as percentage of period(num_cycles/TA0CCR0),
                       must be < TA0CCR0-1
  Return value: true if valid parameter, false if invalid parameter
*/
bool SetDuty_Right(uint16_t num_cycles);

/*
  SetDuty_Left
  ----------------------------------------------------------------------
  Changes the speed of the motor to selected duty cycle.

  Parameters:   1) speed(in clk cycles) of left motor as percentage of period(num_cycles/TA0CCR0),
                       must be < TA0CCR0-1
  Return value: true if valid parameter, false if invalid parameter
*/
bool SetDuty_Left(uint16_t num_cycles);


#endif // RCR_TIMERA0_H__
