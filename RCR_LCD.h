// RCR_LCD.h
// Compatible with MSP432
// Abhi Kallur

// Provides device driver for a Nokia 5110
// LCD screen. The LCD is attached to the rear
// of the robot and can display debug data
// on a 48x84 screen. Communication is done
// through 4-wire SPI.


// Adafruit 338 Nokia 5110
// ---------------
// Ground connected to ground (Gnd, pin 1)
// 3.3V connected to Power (Vcc, pin 2)
// P9.5 (SPI A3 CLK) connected to Clock (Clk, pin 3)
// P9.7 (SPI A3 SIMO) connected to Slave Input (Din, pin 4)
// P9.6 connected to Data/Command (D/C, pin 5)
// P9.4 (SPI A3 CS) connected to Chip Select (CS, pin 6)
// P9.3 connected to Reset (RST, pin 7)
// 3.3V connected to back light (LED, pin 8)


#ifndef RCR_LCD_H_
#define RCR_LCD_H_

#include <stdbool.h>

void LCD_Init(void);

void LCD_Reset(void);

void LCD_WriteChar(char chr);

void LCD_WriteStr(char* string);

void LCD_OutUInt(uint32_t num);

void LCD_ClrScrn(void);

bool LCD_ClrSection(uint8_t Xs, uint8_t Xe, uint8_t Ys, uint8_t Ye);

bool LCD_SetCursor(uint8_t X, uint8_t Y);

#endif /* RCR_LCD_H_ */
