#include <msp430.h> 
#include "imu.h"
#include <stdint.h>

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction

    // IMU test
    imu_init();
    while(1) {
        P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
        vector_3f gyro = imu_read_gyro();

        __delay_cycles(10000);
    }
  }
