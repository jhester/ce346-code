#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure P3.0 as C1 and P1.0 (LED as COUT
    P3SEL1 |= BIT0;
    P3SEL0 |= BIT0;

    // Check pg 87 in datasheet
    P1SEL1 |= BIT1;
    P1SEL0 &= ~BIT1;
    P1DIR |= BIT1;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Setup Comparator_E
    CECTL0 = CEIPEN | CEIPSEL_12;           // Enable V+, input channel CE12
    CECTL1 = CEPWRMD_1;                     // normal power mode
    CECTL2 = CEREFL_2 | CERS_3 | CERSEL;    // VREF is applied to -terminal
                                            // R-ladder off; bandgap ref voltage
                                            // supplied to ref amplifier to get Vcref=2.0V
    CECTL3 = CEPD12;                        // Input Buffer Disable @P3.0/CE12, see section 35.2.7
    CECTL1 |= CEON;                         // Turn On Comparator_E (finally)

    __delay_cycles(75);                     // delay for the reference to settle

    __bis_SR_register(LPM4_bits);           // Enter LPM4
    __no_operation();                       // For debug
}
