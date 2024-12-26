#ifndef _QMC5883L_H_
#define _QMC5883L_H_

#include <stdint.h>
#include "I2Cdev.h"
#include <math.h>

#define QMC5883_ADDR 0x0D

// Output Data Registers
#define QMC5883L_DATA_READ_X_LSB    0x00
#define QMC5883L_DATA_READ_X_MSB    0x01
#define QMC5883L_DATA_READ_Y_LSB    0x02
#define QMC5883L_DATA_READ_Y_MSB    0x03
#define QMC5883L_DATA_READ_Z_LSB    0x04
#define QMC5883L_DATA_READ_Z_MSB    0x05

// Status Register
#define QMC5883L_STATUS             0x06 // DOR | OVL | DRDY

// Temperature Data Registers
#define QMC5883L_TEMP_READ_LSB      0x07
#define QMC5883L_TEMP_READ_MSB      0x08

// Control Registers
#define QMC5883L_CONTROL_1          0x09 // OSR | RNG | ODR | MODE
#define QMC5883L_CONTROL_2          0x0A // SOFT_RST | ROL_PNT | INT_ENB
#define QMC5883L_PERIOD             0x0B // SET/RESET Period FBR [7:0]
#define QMC5883L_CHIP_ID            0x0D

// CONTROL REGISTER 1
#define MODE_STANDBY                0b00000000
#define MODE_CONTINUOUS             0b00000001

#define ODR_10HZ                    0b00000000
#define ODR_50HZ                    0b00000100
#define ODR_100HZ                   0b00001000
#define ODR_200HZ                   0b00001100

#define RNG_2G                      0b00000000
#define RNG_8G                      0b00010000

#define OSR_512                     0b00000000
#define OSR_256                     0b01000000
#define OSR_128                     0b10000000
#define OSR_64                      0b11000000

// CONTROL REGISTER 2
#define SOFT_RST                    0b10000000
#define INT_ENB_DIS                 0b00000001

// Status Register
#define STATUS_READY                0x01

// Calibration structure
typedef struct {
    struct {
        int min;
        int max;
    } axes[3];
} Calibration;

// Raw data structure
typedef struct {
    int x;
    int y;
    int z;
} RawDataAxes;

typedef struct // Axes raw data, returned by rawDataArray()
   {
      int axes[3];
   }RawDataArray;

// Scaled data structure
typedef struct {
    float x;
    float y;
    float z;
} CalibratedDataAxes;

 typedef struct// Axes scaled/calibrated data, returned by rawDataArray()
   {
      float axes[3];
   }CalibratedDataArray;

// QMC5883L Device Structure
typedef struct {
    uint8_t devAddr;
    uint8_t buffer[6];
    Calibration calibration;
    long biases[3];
    float scales[3];
} QMC5883L;

// Function declarations
void QMC5883L_Init(QMC5883L *qmc, uint8_t address);

void QMC5883L_ResetCalibration(QMC5883L *qmc);

void QMC5883L_recalcCalibration(QMC5883L *qmc);

void QMC5883L_Begin(QMC5883L *qmc);

void QMC5883L_SetMode(QMC5883L *qmc, uint8_t mode, uint8_t odr, uint8_t rng, uint8_t osr);

void QMC5883L_SetConfig(QMC5883L *qmc);

void QMC5883L_SetCalibration(QMC5883L *qmc, const Calibration *calib);

int16_t QMC5883L_rawDataAxis(QMC5883L *qmc, uint8_t axis);

int16_t QMC5883L_rawDataX(QMC5883L *qmc);

int16_t QMC5883L_rawDataY(QMC5883L *qmc);

int16_t QMC5883L_rawDataZ(QMC5883L *qmc);

RawDataAxes QMC5883L_RawDataAxes(QMC5883L *qmc);

RawDataArray QMC5883L_rawDataArray(QMC5883L *qmc);

float QMC5883L_calibratedDataAxis(QMC5883L *qmc, uint8_t axis);

float QMC5883L_calibratedDataX(QMC5883L *qmc);

float QMC5883L_calibratedDataY(QMC5883L *qmc);

float QMC5883L_calibratedDataZ(QMC5883L *qmc);

CalibratedDataAxes QMC5883L_calibratedDataAxes(QMC5883L *qmc);

CalibratedDataArray QMC5883L_calibratedDataArray(QMC5883L *qmc);

float QMC5883L_AzimuthZUp(QMC5883L *qmc);


#endif
