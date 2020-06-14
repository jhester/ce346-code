#include <msp430.h> 


/**
 * blink.c -- Make the red LED blink
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;       // disable the GPIO power-on default high-impedance mode
	
	// Set the mode to output mode for Pin 0 on Port 1
	P1DIR |= BIT0;
	P1OUT &= ~BIT0; // set low

    while(1) {
        P1OUT &= ~BIT0; // LED off
        __delay_cycles(100000);
        P1OUT |= BIT0; // LED on
        __delay_cycles(100000);

        // or just do this with XOR
        //P1OUT ^= BIT0; // LED on
        //__delay_cycles(100000);

    }

}
