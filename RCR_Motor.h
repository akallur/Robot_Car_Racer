// RCR_Motor.h
// Compatible with MSP432
// Abhi Kallur

// Provide device drivers for 2 DC brushed
// motors that are interfaced through GPIO
// and H-bridge IC's. PWM adjusts speed of
// both motors.

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




#ifndef RCR_MOTOR_H_
#define RCR_MOTOR_H_


//#include <stdbool.h>


/*
 Hardware connections
 ---------------------------------------------------------
 P5.4 connected to Left motor direction
 P2.7 (TimerA0 sub 4) connected to Left motor PWM
 P3.7 connected to Left motor enable
 P5.5 connected to Right motor direction
 P2.6 (TimerA0 sub 3) connected to Right motor PWM
 P3.6 connected to Right motor enable
*/

#define FORWARD      0x00
#define BACKWARD     0x30
#define RIGHTWARD    0x20
#define LEFTWARD     0x10


/*
  Motor_Init
  ----------------------------------------------------------------------
  Initialize GPIO pins for output, which will be used to control the
  direction of the motors and to enable or disable the driver IC's.
  The motors are initially stopped, the driver IC's are initially powered
  down, and the PWM speed control is uninitialized.

  Timer A0 is initialized for a 15000 clock cycle period that is 1/10
  of the motor time constants of 100 ms.

  Parameters:   none
  Return value: none
*/
void Motor_Init(void);

/*
  Motor_Stop
  ----------------------------------------------------------------------
  Stop the motors by powering down the driver IC's.

  Parameters:   none
  Return value: none
*/
void Motor_Stop(void);

/*
  Motor_Forward
  ----------------------------------------------------------------------
  Drive the motors forward by clearing the direction pins and by running
  left and right motors with the given duty cycles. Assumes Motor_Init()
  has been called.

  Motor direction is negative logic where forward is a 0 on I/O pins.

  Parameters:   1) speed(in clk cycles) of left motor as percentage of period(leftDuty/15000),
                       must be <= 14,998
                2) speed(in clk cycles) of right motor as percentage of period(rightDuty/15000),
                       must be <= 14,998
  Return value: none
*/
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty);

/*
  Motor_Right
  ----------------------------------------------------------------------
  Turn the robot to the right by running the left motor forward and the
  right motor backward with the given duty cycles. Assumes Motor_Init()
  has been called.

  Motor direction is negative logic where forward is a 0 on I/O pins.

  Parameters:   1) speed(in clk cycles) of left motor as percentage of period(leftDuty/15000),
                       must be <= 14,998
                2) speed(in clk cycles) of right motor as percentage of period(rightDuty/15000),
                       must be <= 14,998
  Return value: none
*/
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty);

/*
  Motor_Left
  ----------------------------------------------------------------------
  Turn the robot to the left by running the left motor backward and the
  right motor forward with the given duty cycles. Assumes Motor_Init()
  has been called.

  Motor direction is negative logic where forward is a 0 on I/O pins.

  Parameters:   1) speed(in clk cycles) of left motor as percentage of period(leftDuty/15000),
                       must be <= 14,998
                2) speed(in clk cycles) of right motor as percentage of period(rightDuty/15000),
                       must be <= 14,998
  Return value: none
*/
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty);

/*
  Motor_Backward
  ----------------------------------------------------------------------
  Drive the robot backward by running left and right motors with
  the given duty cycles and setting the direction pins. Assumes Motor_Init()
  has been called.

  Motor direction is negative logic where forward is a 0 on I/O pins.

  Parameters:   1) speed(in clk cycles) of left motor as percentage of period(leftDuty/15000),
                       must be <= 14,998
                2) speed(in clk cycles) of right motor as percentage of period(rightDuty/15000),
                       must be <= 14,998
  Return value: none
*/
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty);

/*
  Motor_Direction
  ----------------------------------------------------------------------
  Gives the current direction of the motors. Assumes Motor_Init()
  has been called.

  Motor direction is negative logic where forward is a 0 on I/O pins.

  Parameters:   none
  Return value: 0x00 is forward,
                0x30 is backward,
                0x20 is right,
                0x10 is left
*/
uint8_t Motor_Direction(void);

#endif /* RCR_MOTOR_H_ */
