#include <msp430.h> 
/**
 * This uses the TIMERA peripheral to output a PWM signal on P1.0
 * Disconect the LED jumper on your launchpad and connect it to
 * the logic analyzer and an external LED -> 330 ohm resistor -> GND
 *
 * @author Josiah Hester
 */


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;                   // P1.0
    P1SEL0 |= BIT0;                  // P1.0

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK=VLO SMCLK=DCO
    CSCTL3 = DIVA__8 | DIVS__8;   // Set all dividers
                                  // ACLK=1000, SMCLK = 1MHz
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure Timer0_A
    TA0CCR0 = 1000-1;                       // PWM Period
    TA0CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA0CCR1 = 750;                          // CCR1 PWM duty cycle
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR

}
