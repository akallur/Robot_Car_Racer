// RCR_SPI_A3.h
// Compatible with MSP432
// Abhi Kallur

// Initializes SPI A3 module specifically for
// a Nokia 5110 LCD screen. For now, only
// a transfer function is provided since an LCD
// will only receive and not transmit back to the
// microcontroller.


#ifndef RCR_SPI_A3_H_
#define RCR_SPI_A3_H_

// P9.5 (SPI A3 CLK) connected to Clock (Clk, pin 3)
// P9.7 (SPI A3 SIMO) connected to Slave Input (Din, pin 4)
// P9.4 (SPI A3 CS) connected to Chip Select (CS, pin 6)


void SPI_A3_Init(void);

void SPI_A3_Tx(uint8_t data);

#endif /* RCR_SPI_A3_H_ */
