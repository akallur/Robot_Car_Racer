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

/*
 Hardware connections
 ---------------------------------------------------------
 P9.5 (SPI A3 CLK) connected to Clock (Clk, pin 3)
 P9.7 (SPI A3 SIMO) connected to Slave Input (Din, pin 4)
 P9.4 (SPI A3 CS) connected to Chip Select (CS, pin 6)
*/


/*
  SPI_A3_Init
  ----------------------------------------------------------------------
  Initialize SPI A3 for 48x84 LCD and its related software functions. The
  clock is set to the SMCLK(12 MHz)/3 to have a 4 MHz baud rate for the LCD.
  No SPI interrupts are needed since communication happens infrequently.
  The STE pin is configured to be negative logic and will enable the slave.
  The clock signal is set to do a transmit on the first edge and is high
  when inactive.

  Parameters:   none
  Return value: none
*/
void SPI_A3_Init(void);

/*
  SPI_A3_Tx
  ----------------------------------------------------------------------
  Transmit 1 byte of data on SPI bus. It'll wait for SPI bus to go to
  ready and then load TX buffer with the data.

  Parameters:   1) 8-bit data to be transmitted through SPI
  Return value: none
*/
void SPI_A3_Tx(uint8_t data);

#endif /* RCR_SPI_A3_H_ */
