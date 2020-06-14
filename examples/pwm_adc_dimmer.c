#include <msp430.h> 

/**
 * This reads the ADC coming in from a potentiometer to set the PWM duty cycle
 */


// A variable should be declared volatile whenever its value
// could change unexpectedly, like...
// ...when reading memory-mapped peripheral registers
volatile unsigned int ADCvar;



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;                   // P1.0
    P1SEL0 |= BIT0;                  // P1.0
    // Configure GPIO for reading potentiometer
    P3SEL1 |= BIT0;                         // Enable A/D channel A12
    P3SEL0 |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK=VLO SMCLK=DCO
    CSCTL3 = DIVA__8 | DIVS__8;   // Set all dividers
                                  // ACLK=1000, SMCLK = 1MHz
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure ADC12
    ADC12CTL0 = ADC12ON | ADC12SHT0_2;      // Turn on ADC12, set sampling time
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12MCTL0 = ADC12INCH_12 | ADC12VRSEL_0; // Vr+ = AVCC and Vr-=AVss
    ADC12CTL0 |= ADC12ENC;                  // Enable conversions

    // Configure Timer0_A
    TA0CCR0 = 1000-1;                       // PWM Period
    TA0CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA0CCR1 = 750;                          // CCR1 PWM duty cycle
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR

    while (1)
    {
        ADC12CTL0 |= ADC12SC;               // Start conversion-software trigger
        while (!(ADC12IFGR0 & BIT0));
        ADCvar = ADC12MEM0;                 // Read conversion result (memmap peripheral reg)
        TA0CCR1 = ADCvar>>2;
        __no_operation();                   // SET BREAKPOINT HERE
        __delay_cycles(50);
    }
}
