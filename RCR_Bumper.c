// RCR_Bumper.c
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
//#include "RCR_SysTick.h"


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


static void (*Port4Task)(uint8_t);   // user function called by interrupt

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
void Bump_Init(void(*task)(uint8_t)) {
    Port4Task = task;
    //SysTick_Init();

    P4->SEL0 &= ~0xED;
    P4->SEL1 &= ~0xED;
    P4->DIR  &= ~0xED;
    P4->REN  |=  0xED;
    P4->OUT  |=  0xED;           //internal pullup
    P4->IES  |=  0xED;           // falling edge
    P4->IFG  &= ~0xED;
    P4->IE   |=  0xED;

    NVIC->IP[38]   = 0x00;  //priority 0, collisions are highest priority in robot
    NVIC->ISER[1] |= 0x00000040;
}


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
uint8_t Bump_Read(void) {
    return  (uint8_t)(~((P4->IN&0x01) | ((P4->IN&0x0C)>>1) | ((P4->IN&0xE0)>>2)));
}


/*
  PORT4_IRQHandler
  ----------------------------------------------------------------------
  Edge-triggered interrupt that occurs on a falling edge of a bump switch.
  The bump switches are read and then the user task is called to handle
  any collisions. Multiple switch interrupts can be handled simultaneously.

  Parameters:   none
  Return value: none
*/
void PORT4_IRQHandler(void) {
    //uint32_t before = SysTick->VAL;
    //P1->OUT ^= 0x01;      //performance measuring
    //P1->OUT ^= 0x01;

    uint8_t status = P4->IV;
    if (status == 2)     // bit 0
    {
        P4->IFG  &= ~0x01;
        status = P4->IV;
    }
    if (status == 6)     // bit 2
    {
        P4->IFG  &= ~0x04;
        status = P4->IV;
    }
    if (status == 8)     //bit 3
    {
        P4->IFG  &= ~0x08;
        status = P4->IV;
    }
    if (status == 12)     // bit 5
    {
        P4->IFG  &= ~0x20;
        status = P4->IV;
    }
    if (status == 14)     //bit 6
    {
        P4->IFG  &= ~0x40;
        status = P4->IV;
    }
    if (status == 16)     //bit 7
    {
        P4->IFG  &= ~0x80;
    }

    (*Port4Task)(Bump_Read());
    //P1->OUT ^= 0x01;
    //uint32_t after = SysTick->VAL;
    //uint32_t elapsed_t = before - after;
}

