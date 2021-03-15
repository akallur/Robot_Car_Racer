// RCR_Bumper.h
// Compatible with MSP432
// Abhi Kallur

// Provides device driver for 6 switches
// that act as bump sensors on the front of
// the robot. They are meant to detect collisions
// and put the robot in the appropriate recovery
// state.

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

#ifndef RCR_BUMPER_H__
#define RCR_BUMPER_H__


/*
 Hardware connections
 ---------------------------------------------------------
 Negative logic bump sensors with internal pullups
 P4.7 connected to Bump5, left side of robot
 P4.6 connected to Bump4
 P4.5 connected to Bump3
 P4.3 connected to Bump2
 P4.2 connected to Bump1
 P4.0 connected to Bump0, right side of robot
*/


/*
  Bump_Init
  ----------------------------------------------------------------------
  Initialize Bump sensors with negative logic. Port 4 pins 7,6,5,3,2,0
  are all set to inputs and have internal pullup resistors. Interrupts
  occur on falling edge(down press) and will call user function, which
  will shut off the motors.

  Edge-triggered interrupt is used in handling robot collisions and
  has the highest priority in the system.

  Parameters:   1) function pointer to user task to be called during interrupt
  Return value: none
*/
void Bump_Init(void(*task)(uint8_t));


/*
  Bump_Read
  ----------------------------------------------------------------------
  Reads current state of all switches and returns a 6-bit positive
  logic result.

  Parameters:   none
  Return value: state of all bump switches,
                    bit 5 = Bump5(leftmost),
                    bit 4 = Bump4,
                    bit 3 = Bump3,
                    bit 2 = Bump2,
                    bit 1 = Bump1,
                    bit 0 = Bump0(rightmost),
                    range is 0 to 63
*/
uint8_t Bump_Read(void);

#endif // RCR_BUMPER_H__
