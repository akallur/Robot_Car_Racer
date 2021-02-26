
// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to P9.3
// UCA3STE       (CE,  pin 2) connected to P9.4
// Data/Command  (DC,  pin 3) connected to P9.6
// UCA3SIMO      (Din, pin 4) connected to P9.7
// UCA3CLK       (Clk, pin 5) connected to P9.5
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 3.3 V white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground


#ifndef LCD_H_
#define LCD_H_

#include <stdbool.h>

void LCD_Init(void);

void LCD_Reset(void);

void LCD_WriteChar(char chr);

void LCD_WriteStr(char* string);

void LCD_OutUInt(uint32_t num);

void LCD_ClrScrn(void);

bool LCD_ClrSection(uint8_t Xs, uint8_t Xe, uint8_t Ys, uint8_t Ye);

bool LCD_SetCursor(uint8_t X, uint8_t Y);

#endif /* LCD_H_ */
