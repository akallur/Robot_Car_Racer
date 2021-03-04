// RCR_SPI_A3.c
// Compatible with MSP432
// Abhi Kallur

// Initializes SPI A3 module specifically for
// a Nokia 5110 LCD screen. For now, only
// a transfer function is provided since an LCD
// will only receive and not transmit back to the
// microcontroller.


#include <stdint.h>
#include "msp.h"


// P9.5 (SPI A3 CLK) connected to Clock (Clk, pin 3)
// P9.7 (SPI A3 SIMO) connected to Slave Input (Din, pin 4)
// P9.4 (SPI A3 CS) connected to Chip Select (CS, pin 6)

// Initialize SPI A3 for 48x84 LCD and its
// related software functions

void SPI_A3_Init(void) {
    EUSCI_A3->CTLW0 |= 0x0001;    //disable module to allow for config
    //Tx on first edge, CLK is high when idle, MSB first, 8-bit data, master, STE active low,
    //synchronous 12MHz SMCLK, STE enables slave
    EUSCI_A3->CTLW0  = 0x6D83;
    EUSCI_A3->BRW    = 0x0003;    //LCD max baud rate is 4 MHz so SMCLK/3 = 4 MHz
    EUSCI_A3->MCTLW  = 0x0000;    //no modulation for SPI
    EUSCI_A3->IE     = 0x0000;    //no SPI A3 interrupts
    EUSCI_A3->IFG    = 0x0000;

    P9->SEL0 |=  0xB0;            //configure 9.4,9.5,9.7 for SPI
    P9->SEL1 &= ~0xB0;

    EUSCI_A3->CTLW0 &= ~0x0001;   //release module for operation
}

void SPI_A3_Tx(uint8_t data) {
    while(EUSCI_A3->STATW&0x0001 == 1) {}   //wait for SPI to be ready and then tx
    EUSCI_A3->TXBUF = data;
}
