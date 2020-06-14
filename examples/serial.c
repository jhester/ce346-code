#include <msp430.h> 
#include <stdint.h>


void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode

	// P2.1/UCA0RXD (to MCU from the desktop)
	// P2.0.UCA0TXD (from MCU to the desktop)
	P2SEL1 |= (BIT1 | BIT0);
	P2SEL0 &= ~(BIT1 | BIT0);

	UCA0CTLW0 = UCSWRST;
	// Baud rate!
    UCA0CTLW0 |= UCSSEL_2;             // CLK = SMCLK
	UCA0BRW = 6;//UCBRx;
	// UCBRSx = 0x20
	UCA0MCTLW |= UCOS16 | UCBRF_6 | 0x2000;
	UCA0CTLW0 &= ~UCSWRST; // Release eUSCI for operation
	while(1) {
	    while(!(UCA0IFG & UCTXIFG)); // busy wait until eUSCI_A0 buffer is ready
	    UCA0TXBUF ='a';
	    __delay_cycles(100000);
	}

}
