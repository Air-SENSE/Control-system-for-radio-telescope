#include "mpu6050.h"
#include <string.h>
#include <math.h>

#define I2C_NUM I2C_NUM_0

#define RAD_TO_DEG (180.0/M_PI)
#define DEG_TO_RAD 0.0174533

void MPU6050_init(MPU6050 *mpu, uint8_t address) {
    mpu->devAddr = address;
    memset(mpu->buffer, 0, sizeof(mpu->buffer));
    MPU6050_setClockSource(mpu, MPU6050_CLOCK_PLL_XGYRO);
    MPU6050_setFullScaleGyroRange(mpu, MPU6050_GYRO_FS_250);
    MPU6050_setFullScaleAccelRange(mpu, MPU6050_ACCEL_FS_2);
    MPU6050_setSleepEnabled(mpu, false);
}

uint8_t MPU6050_getRate(MPU6050 *mpu) {
    I2Cdev_readByte(mpu->devAddr, MPU6050_RA_SMPLRT_DIV, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setRate(MPU6050 *mpu, uint8_t rate) {
    I2Cdev_writeByte(mpu->devAddr, MPU6050_RA_SMPLRT_DIV, rate);
}

uint8_t MPU6050_getExternalFrameSync(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setExternalFrameSync(MPU6050 *mpu, uint8_t sync) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, sync);
}

uint8_t MPU6050_getDLPFMode(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setDLPFMode(MPU6050 *mpu, uint8_t mode) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

uint8_t MPU6050_getFullScaleGyroRange(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setFullScaleGyroRange(MPU6050 *mpu, uint8_t range) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

uint8_t MPU6050_getFullScaleAccelRange(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setFullScaleAccelRange(MPU6050 *mpu, uint8_t range) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

bool MPU6050_getI2CBypassEnabled(MPU6050 *mpu) {
    I2Cdev_readBit(mpu->devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setI2CBypassEnabled(MPU6050 *mpu, bool enabled) {
    I2Cdev_writeBit(mpu->devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

bool MPU6050_getSleepEnabled(MPU6050 *mpu) {
    I2Cdev_readBit(mpu->devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setSleepEnabled(MPU6050 *mpu, bool enabled) {
    I2Cdev_writeBit(mpu->devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

uint8_t MPU6050_getClockSource(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setClockSource(MPU6050 *mpu, uint8_t source) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

uint8_t MPU6050_getDeviceID(MPU6050 *mpu) {
    I2Cdev_readBits(mpu->devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, mpu->buffer, I2Cdev_readTimeout);
    return mpu->buffer[0];
}

void MPU6050_setDeviceID(MPU6050 *mpu, uint8_t id) {
    I2Cdev_writeBits(mpu->devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, id);
}

void MPU6050_getRawMotion6(MPU6050 *mpu, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    I2Cdev_readBytes(mpu->devAddr, MPU6050_RA_ACCEL_XOUT_H, 14, mpu->buffer, I2Cdev_readTimeout);
    *ax = (((int16_t)mpu->buffer[0]) << 8) | mpu->buffer[1];
    *ay = (((int16_t)mpu->buffer[2]) << 8) | mpu->buffer[3];
    *az = (((int16_t)mpu->buffer[4]) << 8) | mpu->buffer[5];
    *gx = (((int16_t)mpu->buffer[8]) << 8) | mpu->buffer[9];
    *gy = (((int16_t)mpu->buffer[10]) << 8) | mpu->buffer[11];
    *gz = (((int16_t)mpu->buffer[12]) << 8) | mpu->buffer[13];
}

void MPU6050_getMotion6(MPU6050 *mpu, double *_ax, double *_ay, double *_az, double *_gx, double *_gy, double *_gz, float accel_sensitivity, float gyro_sensitivity)
{
    int16_t ax,ay,az;
	int16_t gx,gy,gz;
    MPU6050_getRawMotion6(mpu, &ax, &ay, &az, &gx, &gy, &gz);
    
    *_ax = (double)ax / accel_sensitivity;
	*_ay = (double)ay / accel_sensitivity;
	*_az = (double)az / accel_sensitivity;
    *_gx = (double)gx / gyro_sensitivity;
	*_gy = (double)gy / gyro_sensitivity;
	*_gz = (double)gz / gyro_sensitivity;
}

float MPU6050_getElevation(MPU6050 *mpu, double accX, double accY, double accZ) {
    return atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
}