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

// P5.4 connected to Left motor direction
// P2.7 (TimerA0 sub 4) connected to Left motor PWM
// P3.7 connected to Left motor enable
// P5.5 connected to Right motor direction
// P2.6 (TimerA0 sub 3) connected to Right motor PWM
// P3.6 connected to Right motor enable

#include <stdbool.h>


/**
 * Initialize GPIO pins for output, which will be
 * used to control the direction of the motors and
 * to enable or disable the drivers.
 * The motors are initially stopped, the drivers
 * are initially powered down, and the PWM speed
 * control is uninitialized.
 */
void Motor_Init(void);

/**
 * Stop the motors, power down the drivers, and
 * set the PWM speed control to 0% duty cycle.
 */
void Motor_Stop(void);

/**
 * Drive the robot forward by running left and
 * right wheels forward with the given duty
 * cycles. Assumes Motor_Init() has been called.
 * 0 <= leftDuty <= 14,998
 * 0 <= rightDuty <= 14,998
 */
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty);

/**
 * Turn the robot to the right by running the
 * left wheel forward and the right wheel
 * backward with the given duty cycles.
 * Assumes Motor_Init() has been called.
 * 0 <= leftDuty <= 14,998
 * 0 <= rightDuty <= 14,998
 */
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty);

/**
 * Turn the robot to the left by running the
 * left wheel backward and the right wheel
 * forward with the given duty cycles.
 * Assumes Motor_Init() has been called.
 * 0 <= leftDuty <= 14,998
 * 0 <= rightDuty <= 14,998
 */
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty);

/**
 * Drive the robot backward by running left and
 * right wheels backward with the given duty
 * cycles. Assumes Motor_Init() has been called.
 * 0 <= leftDuty <= 14,998
 * 0 <= rightDuty <= 14,998
 */
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty);

bool Motor_Direction(void);

#endif /* RCR_MOTOR_H_ */
