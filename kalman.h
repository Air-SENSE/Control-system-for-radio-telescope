#ifndef _KALMAN_H_
#define _KALMAN_H_

#include <stdint.h>

typedef struct {
    /* Kalman filter variables */
    float Q_angle;      // Process noise variance for the accelerometer
    float Q_bias;       // Process noise variance for the gyro bias
    float R_measure;    // Measurement noise variance

    float angle;        // The angle calculated by the Kalman filter
    float bias;         // The gyro bias calculated by the Kalman filter
    float rate;         // Unbiased rate calculated from the rate and the bias

    float P[2][2];      // Error covariance matrix (2x2 matrix)
} Kalman;

// Initialize the Kalman filter
void Kalman_Init(Kalman *kalman);

// Get the current angle using Kalman filter
float Kalman_GetAngle(Kalman *kalman, float newAngle, float newRate, float dt);

// Set the initial angle
void Kalman_SetAngle(Kalman *kalman, float angle);

// Get the unbiased rate
float Kalman_GetRate(Kalman *kalman);

// Set the process noise variance for the accelerometer
void Kalman_SetQangle(Kalman *kalman, float Q_angle);

// Set the process noise variance for the gyro bias
void Kalman_SetQbias(Kalman *kalman, float Q_bias);

// Set the measurement noise variance
void Kalman_SetRmeasure(Kalman *kalman, float R_measure);

// Get the process noise variance for the accelerometer
float Kalman_GetQangle(Kalman *kalman);

// Get the process noise variance for the gyro bias
float Kalman_GetQbias(Kalman *kalman);

// Get the measurement noise variance
float Kalman_GetRmeasure(Kalman *kalman);

#endif
