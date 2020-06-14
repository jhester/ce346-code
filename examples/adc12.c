#include <msp430.h> 
#include <stdint.h>

// A variable should be declared volatile whenever its value
// could change unexpectedly, like...
// ...when reading memory-mapped peripheral registers
volatile unsigned int ADCvar;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P3SEL1 |= BIT0;                         // Enable A/D channel A12
    P3SEL0 |= BIT0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure ADC12
    ADC12CTL0 = ADC12ON | ADC12SHT0_2;      // Turn on ADC12, set sampling time
    ADC12CTL1 = ADC12SHP;                   // Use sampling timer
    ADC12MCTL0 = ADC12INCH_12 | ADC12VRSEL_0; // Vr+ = AVCC and Vr-=AVss
    ADC12CTL0 |= ADC12ENC;                  // Enable conversions

    while (1)
    {
        ADC12CTL0 |= ADC12SC;               // Start conversion-software trigger
        while (!(ADC12IFGR0 & BIT0));
        ADCvar = ADC12MEM0;                 // Read conversion result (memmap peripheral reg)
        __no_operation();                   // SET BREAKPOINT HERE

    }
}
