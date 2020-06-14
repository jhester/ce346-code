#include <msp430.h>

// Simplest timer
// @author Josiah Hester
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 / LED to output direction (BIT0)

    // Start configuring the Timer!
    TA0CCTL0 = CCIE;                        // TACCR0 interrupt enabled
    TA0CTL = TASSEL_2 + MC_2 + ID_2;        // SMCLK/ID_x, continuous mode (MC_2)
    __bis_SR_register(GIE);                 // Turn on interrupts
}

// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  P1OUT ^= BIT0;                            // Toggle P1.0
}