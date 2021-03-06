// RCR_LCD.c
// Compatible with MSP432
// Abhi Kallur

// Provides device driver for a Nokia 5110
// LCD screen. The LCD is attached to the rear
// of the robot and can display debug data
// on a 48x84 screen. Communication is done
// through 4-wire SPI.


#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "msp.h"
#include "RCR_SPI_A3.h"
#include "RCR_LCD.h"

#define DC          (*((volatile uint8_t *)0x42099058))   //directly accesses 9.6
#define RESET       (*((volatile uint8_t *)0x4209904C))   //directly accesses 9.3

#define PXLS_W      5       //pixel width of a character
#define LCD_ROWS    6
#define LCD_COLUMNS 84

//LCD commands
#define FUNC_EXT     0x21    //active chip, horizontal addressing, extended instructions
#define TEMP_COEFF0  0x04    //set temp coeffcient 0
#define BIAS_1_48    0x13    //set bias to mux rate of 1 : 48
#define CONTRAST_LVL 0xB7    //adjust contrast from 0xA0 to 0xCF

#define FUNC_BASIC   0x20    //active chip, horizontal addressing, basic instructions
#define DISP_NORM    0x0C    //normal display
#define DISP_INV     0x0D    //inverted display



/*
 Hardware connections
 ---------------------------------------------------------
 Adafruit 338 Nokia 5110

 Ground connected to ground (Gnd, pin 1)
 3.3V connected to Power (Vcc, pin 2)
 P9.5 (SPI A3 CLK) connected to Clock (Clk, pin 3)
 P9.7 (SPI A3 SIMO) connected to Slave Input (Din, pin 4)
 P9.6 connected to Data/Command (D/C, pin 5)
 P9.4 (SPI A3 CS) connected to Chip Select (CS, pin 6)
 P9.3 connected to Reset (RST, pin 7)
 3.3V connected to back light (LED, pin 8)
*/



// This table contains the hex values that represent pixels
// for a font that is 5 pixels wide and 8 pixels high
static const uint8_t ASCII[][PXLS_W] = {
   {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c '\'
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
//  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
  ,{0x1f, 0x24, 0x7c, 0x24, 0x1f} // 7f UT sign
};


/*
  LCD_Send_Cmd
  ----------------------------------------------------------------------
  Will transmit a device configuration command to the LCD. The D/C pin
  is cleared for command mode and Reset is disabled. There's a 100 ns
  delay to allow set up time for the pins.

  Parameters:   1) 8-bit configuration command to transmit to LCD
  Return value: none
*/
void LCD_Send_Cmd(uint8_t cmd) {
    int k;
    DC    = 0;                      //command and reset signal is negative logic
    RESET = 1;
    for(k = 0; k < 10; k++) {}      //wait ~100ns for set up time for both pins
    SPI_A3_Tx(cmd);
}


/*
  LCD_Send_Data
  ----------------------------------------------------------------------
  Will transmit a byte to be displayed to the LCD. The D/C pin
  is set for data mode and Reset is disabled. There's a 100 ns
  delay to allow set up time for the pins.

  Parameters:   1) 8-bit display data to transmit to LCD
  Return value: none
*/
void LCD_Send_Data(uint8_t data) {
    int k;
    DC    = 1;                      //data signal is positive logic
    RESET = 1;
    for(k = 0; k < 10; k++) {}      //wait ~100ns for set up time for both pins
    SPI_A3_Tx(data);
}

/*
  LCD_Init
  ----------------------------------------------------------------------
  Initialize the pins and configure the LCD for SPI communication. The
  Data/Command and Reset pin are set up for GPIO and the rest of the pins
  are initialized for SPI. Reset pulse is applied to the LCD on power-up
  and commands are sent for configuration. The contrast level can be adjusted
  and characters are displayed horizontally.

  Refer to PCD8544 technical manual for further device configuration.

  Parameters:   none
  Return value: none
*/
void LCD_Init(void) {
    P9->SEL0 &= ~0x48;          //set up 9.3,9.6 as GPIO outputs
    P9->SEL1 &= ~0x48;
    P9->DIR  |=  0x48;

    //reset pulse of at least 100 ns must be applied to LCD within 30 ms of power on
    LCD_Reset();
    SPI_A3_Init();

    LCD_Send_Cmd(FUNC_EXT);     //configure LCD settings
    LCD_Send_Cmd(TEMP_COEFF0);
    LCD_Send_Cmd(BIAS_1_48);
    LCD_Send_Cmd(CONTRAST_LVL);
    LCD_Send_Cmd(FUNC_BASIC);
    LCD_Send_Cmd(DISP_NORM);
    LCD_SetCursor(0,0);
    LCD_ClrScrn();
}

/*
  LCD_WriteChar
  ----------------------------------------------------------------------
  Transmit 5 bytes to display a single character to the LCD screen. A
  character will take 10us to execute and 2us per byte. Characters are
  looked up in a table based on ASCII value.

  Parameters:   1) single character to be displayed to LCD
  Return value: none
*/
void LCD_WriteChar(char chr) {
    int k;
    uint32_t row = ((int)chr)-32;       //calculate array row from ASCII value
    for(k = 0; k < PXLS_W; k++) {
        LCD_Send_Data(ASCII[row][k]);   //look up and transmit character from ASCII array
    }
}

/*
  LCD_WriteStr
  ----------------------------------------------------------------------
  Transmit all characters in a string to LCD screen. Characters will be
  displayed until null character in string is reached.

  Parameters:   1) string of characters to be displayed to LCD
  Return value: none
*/
void LCD_WriteStr(char* string) {
    int k = 0;
    while(string[k] != '\0') {          //traverse through string until a null is hit
        LCD_WriteChar(string[k]);
        k++;
    }
}

/*
  LCD_OutUInt
  ----------------------------------------------------------------------
  Transmit an integer to LCD screen. The number of digits are calculated,
  reversed, and transmitted as ASCII characters.

  A separate function is needed to handle floating points and signed integers.

  Parameters:   1) positive integer to be displayed to LCD
  Return value: none
*/
void LCD_OutUInt(uint32_t num) {
    int k;
    uint32_t temp = num;
    uint32_t digits = (uint32_t)(log10(num)+1);     //calculate number of digits
    uint32_t* arr = (uint32_t*)malloc(digits*sizeof(uint32_t));
    //if(arr == 0x00000000) return false;
    for(k = digits-1; k >= 0; k--){                 //reverse order of digits pulled off
        arr[k] = temp%10;
        temp /= 10;
    }
    for(k = 0; k < digits; k++) {
        LCD_WriteChar((char)(arr[k]+48));           //convert number to ASCII and transmit as char
    }
    free(arr);                                      //free up temporary array memory
}

/*
  LCD_ClrScrn
  ----------------------------------------------------------------------
  Will clear entire LCD screen by transmitting 0x00 to all pixels.
  It takes 1ms to execute and 2us per byte.

  Parameters:   none
  Return value: none
*/
void LCD_ClrScrn(void) {
    int k;
    for(k = 0; k < LCD_COLUMNS*LCD_ROWS; k++) {
        LCD_Send_Data(0x00);
    }
}

/*
  LCD_ClrSection
  ----------------------------------------------------------------------
  Will clear a section of the LCD screen based on desired coordinates.
  Assumes LCD device is configured for horizontal mode. Columns in a single
  row are cleared and then done for the next rows.

  Parameters:   1) x-coordinate to start clearing at,
                      must be between columns 0 and 83 and <= Xe
                2) x-coordinate to end clearing at and is inclusive,
                      must be between columns 0 and 83
                3) y-coordinate to start clearing at,
                      must be between rows 0 and 5 and <= Ye
                4) y-coordinate to end clearing at and is inclusive,
                      must be between rows 0 and 5
  Return value: true if valid coordinates, false if invalid
*/
bool LCD_ClrSection(uint8_t Xs, uint8_t Xe, uint8_t Ys, uint8_t Ye) {
    int k;
    uint8_t xcnt = Xs;
    uint8_t ycnt = Ys;      //error conditions for input parameters
    if(Xs > 83 || Xe > 83 || Ys > 5 || Ye > 5 || Xe < Xs || Ye < Ys) return false;
    LCD_SetCursor(Xs,Ys);
    for(k = 0; k < ((Xe-Xs)+1)*((Ye-Ys)+1); k++) {
        if(xcnt == Xe+1) {          //increment to next row and align cursor if all columns cleared
            xcnt = Xs;
            ycnt++;
            //if(ycnt > Ye) return false;
            LCD_SetCursor(xcnt,ycnt);
        }
        LCD_Send_Data(0x00);
        xcnt++;
    }
    return true;
}

/*
  LCD_SetCursor
  ----------------------------------------------------------------------
  Transmits commands to set the display cursor at the desired coordinates.
  Data will start being displayed horizontally from this position.

  Parameters:   1) x-coordinate to set display cursor,
                      must be between columns 0 and 83
                2) y-coordinate to set display cursor,
                      must be between rows 0 and 5
  Return value: true if valid coordinates, false if invalid
*/
bool LCD_SetCursor(uint8_t X, uint8_t Y) {
    if(X > 83 || Y > 5) return false;     // 0 <= X <= 83, 0 <= Y <= 5
    LCD_Send_Cmd(0x80 | X);
    LCD_Send_Cmd(0x40 | Y);
    return true;
}

/*
  LCD_Reset
  ----------------------------------------------------------------------
  Applies a Reset pulse to the LCD device. Pulse must last 100 ns.

  Parameters:   none
  Return value: none
*/
void LCD_Reset(void) {
    int k;
    RESET = 0;              //reset signal is negative logic
    for(k = 0; k < 20; k++) {}
    RESET = 1;
}


