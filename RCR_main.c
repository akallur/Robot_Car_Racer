// RCR_main.c
// Compatible with MSP432
// Abhi Kallur

// Main program for Robot Car Racer that
// provides the control system and state machine
// for the robot to autonomously navigate itself.


#include "msp.h"
#include "Clock.h"
#include "CortexM.h"
#include "LaunchPad.h"
#include "RCR_LCD.h"
#include "RCR_IRDistance.h"
#include "RCR_TimerA1.h"
#include "RCR_ADC14.h"
#include "RCR_Motor.h"
#include "RCR_Bumper.h"
#include "RCR_SysTick.h"

#define DATA_X    45
#define STOP_DIST 120   //in mm
#define DISP_RATE 60    //multiply this by sample rate(10 ms for now) to get milliseconds

bool debug_mode = true;
uint32_t ADCflag;
uint8_t CollisionData, CollisionFlag;

uint32_t raw_adc_vals[ANALOG_CHNLS] = {0,0,0};   //[0] = right, [1] = center, [2] = left
uint32_t right_filt  = 0;
uint32_t center_filt = 0;
uint32_t left_filt   = 0;


void Handle_Collision(uint8_t bumpSensor) {     //immediately turn off motors if there is a crash
   Motor_Stop();
   CollisionData = bumpSensor;
   CollisionFlag = 1;
}

void Process_ADC_Samples(void) {        //collect samples, run them through filter, and convert to distance
    ADC_In17_14_16(&raw_adc_vals[RIGHT],&raw_adc_vals[CENTER],&raw_adc_vals[LEFT]);
    right_filt  = LowPassFilter(RIGHT,raw_adc_vals[RIGHT]);
    center_filt = LowPassFilter(CENTER,raw_adc_vals[CENTER]);
    left_filt   = LowPassFilter(LEFT,raw_adc_vals[LEFT]);
    right_filt  = ConvertDist(right_filt);
    center_filt = ConvertDist(center_filt);
    left_filt   = ConvertDist(left_filt);
    ADCflag = 1;
}

void main(void) {
    int num_samples = 0;
    Clock_Init48MHz();
    LCD_Init();
    LaunchPad_Init();
    SysTick_Init();
    ADCflag = 0;
    CollisionFlag = 0;
    ADC0_Init_Ch17_14_16();
    ADC_In17_14_16(&raw_adc_vals[RIGHT],&raw_adc_vals[CENTER],&raw_adc_vals[LEFT]);
    LowPassFilter_Init(ANALOG_CHNLS,64,raw_adc_vals);   //the larger the filter size, the more accurate the sample
    TimerA1_Init(&Process_ADC_Samples,1875);     //every 10 ms
    Motor_Init();
    Bump_Init(&Handle_Collision);

    LCD_SetCursor(5,0);
    LCD_WriteStr("Right: ");
    LCD_SetCursor(65,0);
    LCD_WriteStr(" mm");
    LCD_SetCursor(5,2);
    LCD_WriteStr("Center: ");
    LCD_SetCursor(65,2);
    LCD_WriteStr(" mm");
    LCD_SetCursor(5,4);
    LCD_WriteStr("Left: ");
    LCD_SetCursor(65,4);
    LCD_WriteStr(" mm");

    Motor_Forward(5000,5000);   //start at 33% speed
    LaunchPad_LED(0);
    EnableInterrupts();
    while(1) {
        if(ADCflag == 1) {  //will have to turn this logic into a state machine
            ADCflag = 0;
            //handle any potential or actual crashes by stopping and reversing
            if(right_filt < STOP_DIST || center_filt < STOP_DIST || left_filt < STOP_DIST || CollisionFlag) {
                if(Motor_Direction() != BACKWARD) {
                    Motor_Stop();    //don't stop if we're already reversing
                    Motor_Backward(1500,2000);
                }
                CollisionFlag = 0;                  //POTENTIAL BUG FOUND
            }                          //need logic to handle tight spaces that cycle between if else block
            else {  //rudimentary navigation that directs the car to take path of least obstacles
                if(center_filt > right_filt && center_filt > left_filt) Motor_Forward(3000,3000);
                else if(right_filt > center_filt && right_filt > left_filt) Motor_Right(2700,1300);
                else if(left_filt > center_filt && left_filt > right_filt) Motor_Left(1300,2700);
            }
            num_samples++;
        }
        if(num_samples == DISP_RATE && debug_mode) {     //display all IR sensor data on LCD screen
            LaunchPad_LED(1);
            //will need semaphore
            LCD_ClrSection(DATA_X,DATA_X+20,0,0);
            LCD_ClrSection(DATA_X,DATA_X+20,2,2);
            LCD_ClrSection(DATA_X,DATA_X+20,4,4);
            LCD_SetCursor(DATA_X,0);        //displays every 600 ms
            LCD_OutUInt(right_filt);
            LCD_SetCursor(DATA_X,2);
            LCD_OutUInt(center_filt);
            LCD_SetCursor(DATA_X,4);
            LCD_OutUInt(left_filt);

            num_samples = 0;
            LaunchPad_LED(0);
        }
    }
}
