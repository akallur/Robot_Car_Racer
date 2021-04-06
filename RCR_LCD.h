// RCR_LCD.h
// Compatible with MSP432
// Abhi Kallur

// Provides device driver for a Nokia 5110
// LCD screen. The LCD is attached to the rear
// of the robot and can display debug data
// on a 48x84 screen. Communication is done
// through 4-wire SPI(but without SOMI pin).



#ifndef RCR_LCD_H_
#define RCR_LCD_H_

#include <stdbool.h>


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
void LCD_Init(void);

/*
  LCD_Reset
  ----------------------------------------------------------------------
  Applies a Reset pulse to the LCD device. Pulse must last 100 ns.

  Parameters:   none
  Return value: none
*/
void LCD_Reset(void);

/*
  LCD_WriteChar
  ----------------------------------------------------------------------
  Transmit 5 bytes to display a single character to the LCD screen. A
  character will take 10us to execute and 2us per byte. Characters are
  looked up in a table based on ASCII value.

  Parameters:   1) character to be displayed to LCD
  Return value: none
*/
void LCD_WriteChar(char chr);

/*
  LCD_WriteStr
  ----------------------------------------------------------------------
  Transmit all characters in a string to LCD screen. Characters will be
  displayed until null character in string is reached.

  Parameters:   1) string of characters to be displayed to LCD
  Return value: none
*/
void LCD_WriteStr(char* string);

/*
  LCD_OutUInt
  ----------------------------------------------------------------------
  Transmit an integer to LCD screen. The number of digits are calculated,
  reversed, and transmitted as ASCII characters.

  A separate function is needed to handle floating points and signed integers.

  Parameters:   1) positive integer to be displayed to LCD
  Return value: none
*/
void LCD_OutUInt(uint32_t num);

/*
  LCD_ClrScrn
  ----------------------------------------------------------------------
  Will clear entire LCD screen by transmitting 0x00 to all pixels.
  It takes 1ms to execute and 2us per byte.

  Parameters:   none
  Return value: none
*/
void LCD_ClrScrn(void);

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
bool LCD_ClrSection(uint8_t Xs, uint8_t Xe, uint8_t Ys, uint8_t Ye);

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
bool LCD_SetCursor(uint8_t X, uint8_t Y);

#endif /* RCR_LCD_H_ */
