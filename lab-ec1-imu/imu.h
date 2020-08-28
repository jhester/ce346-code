/**
 * Simplified shell of an IMU driver for I2C MPU-6050
 * Extremely common IMU cheap online.
 *
 * @author Josiah Hester
 */
#ifndef IMU_H_
#define IMU_H_


#include <stdint.h>

// Default I2C address of the MPU6050 (0x69 if AD0 pin set to HIGH)
#define MPU6050_DEFAULT_ADDRESS 0x68

// Selected Mpu6050 register addresses (found in the invensense datasheet)
#define MPU6050_REGISTER_SMPRT_DIV 0x19 // Sample rate divider
#define MPU6050_REGISTER_CONFIG 0x1A // DLPF config
#define MPU6050_REGISTER_GYRO_CONFIG 0x1B
#define MPU6050_REGISTER_ACCEL_CONFIG 0x1C
#define MPU6050_REGISTER_ACCEL_XOUT_H 0x3B // Accelerometer measurement
#define MPU6050_REGISTER_ACCEL_XOUT_L 0x3C
#define MPU6050_REGISTER_ACCEL_YOUT_H 0x3D
#define MPU6050_REGISTER_ACCEL_YOUT_L 0x3E
#define MPU6050_REGISTER_ACCEL_ZOUT_H 0x3F
#define MPU6050_REGISTER_ACCEL_ZOUT_L 0x40
#define MPU6050_REGISTER_TEMP_OUT_H 0x41 // Temperature measurement
#define MPU6050_REGISTER_TEMP_OUT_L 0x42
#define MPU6050_REGISTER_GYRO_XOUT_H 0x43 // Gyroscope measurement
#define MPU6050_REGISTER_GYRO_XOUT_L 0x44
#define MPU6050_REGISTER_GYRO_YOUT_H 0x45
#define MPU6050_REGISTER_GYRO_YOUT_L 0x46
#define MPU6050_REGISTER_GYRO_ZOUT_H 0x47
#define MPU6050_REGISTER_GYRO_ZOUT_L 0x48
#define MPU6050_REGISTER_PWR_MGMT_1 0x6B // Power management
#define MPU6050_REGISTER_WHO_AM_I 0x75 // Contains address of the device (0x68)

// Constant to convert raw temperature to Celsius degrees
#define MPU6050_TEMP_LINEAR_COEF (1.0/340.00)
#define MPU6050_TEMP_OFFSET       36.53

// Constant to convert raw gyroscope to degree/s
#define MPU6050_GYRO_FACTOR_250 (1.0/131.0)
#define MPU6050_GYRO_FACTOR_500  (1.0/65.5)
#define MPU6050_GYRO_FACTOR_1000 (1.0/32.8)
#define MPU6050_GYRO_FACTOR_2000 (1.0/16.4)

// Constant to convert raw acceleration to m/s^2
#define GRAVITATIONAL_CONSTANT_G 9.81
#define MPU6050_ACCEL_FACTOR_2 (GRAVITATIONAL_CONSTANT_G / 16384.0)
#define MPU6050_ACCEL_FACTOR_4 (GRAVITATIONAL_CONSTANT_G / 8192.0)
#define MPU6050_ACCEL_FACTOR_8 (GRAVITATIONAL_CONSTANT_G / 4096.0)
#define MPU6050_ACCEL_FACTOR_16 (GRAVITATIONAL_CONSTANT_G / 2048.0)

typedef struct vec3f {
    float x;
    float y;
    float z;
    uint16_t rawx;
    uint16_t rawy;
    uint16_t rawz;
} vector_3f;

uint8_t imu_init();
vector_3f imu_read_gyro();
vector_3f imu_read_accel();

#endif /* IMU_H_ */
