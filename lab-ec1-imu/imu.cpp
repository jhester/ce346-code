/**
 * Simplified shell of an IMU driver for I2C MPU-6050
 * Extremely common IMU cheap online.
 *
 * @author Josiah Hester
 */
// https://github.com/Th-Havy/Simple-MPU6050-Arduino/blob/master/Mpu6050.cpp
// Datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
// Register Map: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
#include <msp430.h>
#include "imu.h"

uint8_t rx_buffer[16];

void init_i2c() {
    // Configure GPIO
    P1OUT &= ~BIT0;             // Clear P1.0 output latch
    P1DIR |= BIT0;              // For LED

    // Configure interface ports
    P7SEL0 |=  (BIT0 | BIT1);   // Connect ports P7.0 and P7.1 to
    P7SEL1 &= ~(BIT0 | BIT1);   // eUSCI-B1, SDA and SCL, respectively
    P7DIR  &= ~(BIT0 | BIT1);   // Configure as if they are inputs
    UCB2CTLW0 = UCSWRST;        // Reset I2C interface for configuration

    UCB2CTLW0 =                 /* USCI - B2 configuration register */
                UCMST         | // Master mode
                UCSYNC        | // Synchronous mode
                UCMODE_3      | // I2C mode
                UCSSEL__SMCLK | // Select SMCLK
                UCTR          | // Transmitter mode
                UCSWRST       | // Don't release reset (yet)
                0;

    UCB2CTLW1 =
                UCCLTO_0      | // Clock low time-out select (disabled)
                UCASTP_0      | // Automatic STOP condition generation (disabled)
                UCGLIT_0      | // Deglitch time (50ns)
                0;

    UCB2BRW = 10;               // Bit clock divider 1M/10 = 100kHz
    UCB2CTLW0 &= ~UCSWRST;      // Clear reset bit to start operation
    __delay_cycles(50);
}

void i2c_write(uint8_t reg_addr, uint8_t data) {
    UCB2I2CSA = MPU6050_DEFAULT_ADDRESS; // Set slave address
    UCB2CTLW0 |= UCTR | UCTXSTT; // Start request as transmitter

    while(!(UCB2IFG & UCTXIFG)); // Wait until TXBUFF is empty
    UCB2TXBUF = reg_addr; // Send first data byte

    while(UCB1CTLW0 & UCTXSTT); // Wait for slave's response

    if (UCB2IFG & UCNACKIFG) { // If there was no response
    UCB2IFG &= ~UCNACKIFG; // Clear NACK flag
    UCB2CTLW0 |= UCTXSTP; // Request a stop
    while (UCB2CTLW0 & UCTXSTP); // And wait until stop was actually sent
    UCB2IFG &= ~UCSTPIFG; // Clear stop flag (not actually using this..)
    }
    else { // If there was a reply from the slave,
        while(!(UCB2IFG & UCTXIFG));
        UCB2TXBUF = data;
    }

    while(!(UCB2IFG & UCTXIFG)); // Wait for the last byte to be loaded to shift register
    UCB2CTLW0 |= UCTXSTP; // Request a stop condition
    while (UCB2CTLW0 & UCTXSTP); // Wait until stop was sent

    UCB2IFG &= ~UCTXIFG; // Clear TX flag
}

void i2c_read(uint8_t reg_addr, uint8_t quantity) {
    uint8_t i;

    UCB2I2CSA = MPU6050_DEFAULT_ADDRESS;             // Set slave address
    UCB2CTLW0 |= UCTR | UCTXSTT;            // Start request as transmitter

    while(!(UCB2IFG & UCTXIFG));            // Wait until TXBUFF is empty
    UCB2TXBUF = reg_addr;                   // Send first data byte

    while(UCB1CTLW0 & UCTXSTT);             // Wait for slave's response

    while(!(UCB2IFG & UCTXIFG));            // Wait for the last byte to be loaded to shift register
    UCB2CTLW0 |= UCTXSTP;                   // Request a stop condition
    while (UCB2CTLW0 & UCTXSTP);            // Wait until stop was sent

    UCB2IFG &= ~UCTXIFG;                    // Clear TX flag
    UCB2CTLW0 &= ~UCTR;                     // Put I2C bus in receive mode
    UCB2CTLW0 |= UCTXSTT;                   // Start request as a receiver

    for(i=0;i<quantity;i++) {               // Loop until data is collected
        while(!(UCB2IFG & UCRXIFG));        // Wait until RXBUF is full
        // Generate I2C stop condition (with a nack before that, which is handled by hardware automatically)
        // Only do this for the last byte
        if(i == quantity - 1) UCB2CTL1 |= UCTXSTP;
        rx_buffer[i] = UCB2RXBUF;               // Pull data from buffer
    }
    while (UCB2CTLW0 & UCTXSTP);            // Wait until stop was sent

}

uint8_t imu_init() {
    init_i2c();

   // The content of WHO_AM_I is always 0x68, so if the wiring is right and
   // the I2C communication works fine this function should return true
   i2c_read(MPU6050_REGISTER_WHO_AM_I, 1);
   if(rx_buffer[0] != MPU6050_DEFAULT_ADDRESS) return 1;

   // Wakup from sleep / move to active mode
   i2c_write(MPU6050_REGISTER_PWR_MGMT_1, 0b00000000);
   // Set sample rate divider to 7
   i2c_write(MPU6050_REGISTER_SMPRT_DIV, 7);

   return 0;
}

vector_3f imu_read_gyro()
{
    // Read 6 consecutive bytes (2 bytes per axis)
    i2c_read(MPU6050_REGISTER_GYRO_XOUT_H, 6);

    // Each gyro component is composed of 2 concatenated bytes
    vector_3f gyro;
    gyro.rawx = rx_buffer[0] <<8 | rx_buffer[1];
    gyro.rawy = rx_buffer[2] <<8 | rx_buffer[3];
    gyro.rawz = rx_buffer[4] <<8 | rx_buffer[5];

    // Convert each integer value to physical units
    gyro.x = gyro.rawx*MPU6050_GYRO_FACTOR_250;
    gyro.y = gyro.rawy*MPU6050_GYRO_FACTOR_250;
    gyro.z = gyro.rawz*MPU6050_GYRO_FACTOR_250;

    return gyro;
}

vector_3f imu_read_accel()
{
    // Read 6 consecutive bytes (2 bytes per axis)
    i2c_read(MPU6050_REGISTER_ACCEL_XOUT_H, 6);

    // Each accel component is composed of 2 concatenated bytes
    vector_3f accel;
    accel.rawx = rx_buffer[0] <<8 | rx_buffer[1];
    accel.rawy = rx_buffer[2] <<8 | rx_buffer[3];
    accel.rawz = rx_buffer[4] <<8 | rx_buffer[5];
    return accel;
}

