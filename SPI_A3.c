#include <stdint.h>
#include "msp.h"


// Initialize Nokia 5110 48x84 LCD by sending the proper
// commands to the PCD8544 driver.
// Inputs: none
// Outputs: none

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
