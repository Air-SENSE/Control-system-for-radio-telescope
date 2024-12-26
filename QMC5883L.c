#include "QMC5883L.h"

void QMC5883L_Init(QMC5883L *qmc, uint8_t address) {
    qmc->devAddr = address;
}

void QMC5883L_ResetCalibration(QMC5883L *qmc) {
    uint8_t axis;
    for (axis = 0; axis < 3; axis++) {
        qmc->calibration.axes[axis].min = -32768;
        qmc->calibration.axes[axis].max = 32767;
    }
}

void QMC5883L_recalcCalibration(QMC5883L *qmc)
{
   uint8_t  axis;
   long  deltas[3];
   float delta;

   delta = 0;

   for (axis = 0; axis < 3; axis++)
   {
      qmc->biases[axis]  = ((long)qmc->calibration.axes[axis].max + (long)qmc->calibration.axes[axis].min)/2;
      deltas[axis]  =  (long)qmc->calibration.axes[axis].max - (long)qmc->calibration.axes[axis].min   ;
      delta        += deltas[axis];      
   }

   delta = delta / 3;

   for (axis = 0; axis < 3; axis++)
   {
      qmc->scales[axis] = delta / deltas[axis];
   }
}

void QMC5883L_Begin(QMC5883L *qmc) {
    QMC5883L_ResetCalibration(qmc);
    QMC5883L_recalcCalibration(qmc);
    I2Cdev_writeByte(qmc->devAddr, QMC5883L_CONTROL_2, SOFT_RST);
    I2Cdev_writeByte(qmc->devAddr, QMC5883L_PERIOD, 0x01);
}

void QMC5883L_SetMode(QMC5883L *qmc, uint8_t mode, uint8_t odr, uint8_t rng, uint8_t osr) {
    uint8_t config = mode | odr | rng | osr;
    I2Cdev_writeByte(qmc->devAddr, QMC5883L_CONTROL_1, config);
}

void QMC5883L_SetConfig(QMC5883L *qmc) {
    QMC5883L_SetMode(qmc, MODE_CONTINUOUS, ODR_100HZ, RNG_8G, OSR_512);
    I2Cdev_writeByte(qmc->devAddr, QMC5883L_CONTROL_2, INT_ENB_DIS);
}

void QMC5883L_SetCalibration(QMC5883L *qmc, const Calibration *calib) {
    qmc->calibration = *calib;
    QMC5883L_recalcCalibration(qmc);
}

int16_t QMC5883L_rawDataAxis(QMC5883L *qmc, uint8_t axis) {
    I2Cdev_readBytes(qmc->devAddr, QMC5883L_DATA_READ_X_LSB, 6, qmc->buffer, I2Cdev_readTimeout);
    int16_t data = (((int16_t)qmc->buffer[axis * 2 + 1]) << 8) | qmc->buffer[axis * 2];
    return data;
}

int16_t QMC5883L_rawDataX(QMC5883L *qmc)
{
   return QMC5883L_rawDataAxis(qmc, 0);
}

int16_t QMC5883L_rawDataY(QMC5883L *qmc)
{
   return QMC5883L_rawDataAxis(qmc, 1);
}

int16_t QMC5883L_rawDataZ(QMC5883L *qmc)
{
   return QMC5883L_rawDataAxis(qmc, 2);
}

RawDataAxes QMC5883L_RawDataAxes(QMC5883L *qmc) {
    RawDataAxes rawDataAxes;
    rawDataAxes.x = QMC5883L_rawDataAxis(qmc, 0);
    rawDataAxes.y = QMC5883L_rawDataAxis(qmc, 1);
    rawDataAxes.z = QMC5883L_rawDataAxis(qmc, 2);
    return rawDataAxes;
}

RawDataArray QMC5883L_rawDataArray(QMC5883L *qmc)
{
   RawDataArray rawDataArray;

   rawDataArray.axes[0] = QMC5883L_rawDataAxis(qmc, 0);
   rawDataArray.axes[1] = QMC5883L_rawDataAxis(qmc, 1);
   rawDataArray.axes[2] = QMC5883L_rawDataAxis(qmc, 2);
   
   return rawDataArray;
}

float QMC5883L_calibratedDataAxis(QMC5883L *qmc, uint8_t axis)
{
   return qmc->scales[axis] * (QMC5883L_rawDataAxis(qmc, axis) - qmc->biases[axis]);
}

float QMC5883L_calibratedDataX(QMC5883L *qmc)
{
   return QMC5883L_calibratedDataAxis(qmc, 0);
}

float QMC5883L_calibratedDataY(QMC5883L *qmc)
{
   return QMC5883L_calibratedDataAxis(qmc, 1);
}

float QMC5883L_calibratedDataZ(QMC5883L *qmc)
{
   return QMC5883L_calibratedDataAxis(qmc, 2);
}

CalibratedDataAxes QMC5883L_calibratedDataAxes(QMC5883L *qmc)
{
   CalibratedDataAxes calibratedDataAxes;

   calibratedDataAxes.x = QMC5883L_calibratedDataAxis(qmc, 0);
   calibratedDataAxes.y = QMC5883L_calibratedDataAxis(qmc, 1);
   calibratedDataAxes.z = QMC5883L_calibratedDataAxis(qmc, 2);
   
   return calibratedDataAxes;
}

CalibratedDataArray QMC5883L_calibratedDataArray(QMC5883L *qmc)
{
   CalibratedDataArray calibratedDataArray;

   calibratedDataArray.axes[0] = QMC5883L_calibratedDataAxis(qmc, 0);
   calibratedDataArray.axes[1] = QMC5883L_calibratedDataAxis(qmc, 1);
   calibratedDataArray.axes[2] = QMC5883L_calibratedDataAxis(qmc, 2);
   
   return calibratedDataArray;
}
float QMC5883L_AzimuthZUp(QMC5883L *qmc) {
    float alpha;
    alpha = atan2f(QMC5883L_calibratedDataY(qmc), QMC5883L_calibratedDataX(qmc)); // atan2(y, x), not math. arctan2(x, y)! 
    return (alpha < 0 ? alpha + 2 * 3.14159265f : alpha) * 180.0f / 3.14159265f;
}
