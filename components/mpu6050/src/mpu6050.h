
#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "I2Cdev.h"

#undef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))


#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW

#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A

#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C

#define MPU6050_RA_INT_PIN_CFG      0x37

#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40

#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_WHO_AM_I         0x75

#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_XA_ST_BIT           7
#define MPU6050_ACONFIG_YA_ST_BIT           6
#define MPU6050_ACONFIG_ZA_ST_BIT           5
#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
#define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1

#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CLKSEL_BIT         2
#define MPU6050_PWR1_CLKSEL_LENGTH      3

typedef struct {
    uint8_t devAddr;
    uint8_t buffer[14];
} MPU6050;

void MPU6050_init(MPU6050 *mpu, uint8_t address);

// SMPLRT_DIV register
uint8_t MPU6050_getRate(MPU6050 *mpu);
void MPU6050_setRate(MPU6050 *mpu, uint8_t rate);

// CONFIG register
uint8_t MPU6050_getExternalFrameSync(MPU6050 *mpu);
void MPU6050_setExternalFrameSync(MPU6050 *mpu, uint8_t sync);
uint8_t MPU6050_getDLPFMode(MPU6050 *mpu);
void MPU6050_setDLPFMode(MPU6050 *mpu, uint8_t mode);

// GYRO_CONFIG register
uint8_t MPU6050_getFullScaleGyroRange(MPU6050 *mpu);
void MPU6050_setFullScaleGyroRange(MPU6050 *mpu, uint8_t range);

uint8_t MPU6050_getFullScaleAccelRange(MPU6050 *mpu);
void MPU6050_setFullScaleAccelRange(MPU6050 *mpu, uint8_t range);

bool MPU6050_getI2CBypassEnabled(MPU6050 *mpu);
void MPU6050_setI2CBypassEnabled(MPU6050 *mpu, bool enabled);

bool MPU6050_getSleepEnabled(MPU6050 *mpu);
void MPU6050_setSleepEnabled(MPU6050 *mpu, bool enabled);

uint8_t MPU6050_getClockSource(MPU6050 *mpu);
void MPU6050_setClockSource(MPU6050 *mpu, uint8_t source);

// WHO_AM_I register
uint8_t MPU6050_getDeviceID(MPU6050 *mpu);
void MPU6050_setDeviceID(MPU6050 *mpu, uint8_t id);

void MPU6050_getRawMotion6(MPU6050 *mpu, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
void MPU6050_getMotion6(MPU6050 *mpu, double *_ax, double *_ay, double *_az, double *_gx, double *_gy, double *_gz, float accel_sensitivity, float gyro_sensitivity);
float MPU6050_getElevation(MPU6050 *mpu, double accX, double accY, double accZ);


#endif /* _MPU6050_H_ */