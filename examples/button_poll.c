#include <msp430.h> 

/**
 * Blink the LED using polling
 * @author Josiah Hester
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    P1DIR |= BIT0; // 0b00000001
    P1OUT |= BIT0; // 0b00000001

    // Init button pin
    // (assumes BUTTON connects to ground == Digital 0)
    P5DIR &= ~BIT5;     // set to input pin
    P5REN |= BIT5;      // resistor enable
    P5OUT |= BIT5;      // make it pullup

    while(1) {
        if(P5IN & BIT5) {
            // pin high so button is not pressed
            P1OUT &= ~BIT0;  // turn off LED
        } else {
            // pin low so button is pressed
            P1OUT |= BIT0;  // turn on LED
        }

    }

}

