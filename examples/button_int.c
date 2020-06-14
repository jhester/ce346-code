#include <msp430.h> 


/**
 * Interrupt based button pushing action 
 * the lowest power User Interface ever made
 * @author Josiah Hester
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    P1DIR |= BIT0; // LED direction
    P1OUT |= BIT0; // LED ON

    // Init button on P5.5
    P5DIR &= ~BIT5;     // set to input pin
    P5REN |= BIT5;      // resistor enable
    P5OUT |= BIT5;      // make it pullup
    P5DIR &= ~BIT6;     // set to input pin
    P5REN |= BIT6;      // resistor enable
    P5OUT |= BIT6;      // make it pullup


    // Init interrupts!
    P5IE |= (BIT5 + BIT6);  // enable interrupt for P5.5
    P5IES |= (BIT5 + BIT6); // trigger interrupt on a high to low transition
    P5IFG &= ~(BIT5 + BIT6);     // clear the interrupt flag just in case

    __bis_SR_register(LPM3_bits + GIE);
    while(1)   {     
        __no_operation(); // Only reach here if line 39 uncommented.
    }
}

#pragma vector=PORT5_VECTOR
__interrupt void foo(void) {
    P5IFG &= ~BIT5;
    P5IFG &= ~BIT6;
    __no_operation();
   //__bic_SR_register_on_exit(CPUOFF);
}
