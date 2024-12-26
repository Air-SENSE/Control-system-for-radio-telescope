#include "Kalman.h"

void Kalman_Init(Kalman *kalman) {
    /* Initialize variables */
    kalman->Q_angle = 0.001f;
    kalman->Q_bias = 0.003f;
    kalman->R_measure = 0.03f;

    kalman->angle = 0.0f;
    kalman->bias = 0.0f;
    kalman->rate = 0.0f;

    kalman->P[0][0] = 0.0f;
    kalman->P[0][1] = 0.0f;
    kalman->P[1][0] = 0.0f;
    kalman->P[1][1] = 0.0f;
}

float Kalman_GetAngle(Kalman *kalman, float newAngle, float newRate, float dt) {
    // Step 1: Predict
    kalman->rate = newRate - kalman->bias;
    kalman->angle += dt * kalman->rate;

    // Update estimation error covariance
    kalman->P[0][0] += dt * (dt * kalman->P[1][1] - kalman->P[0][1] - kalman->P[1][0] + kalman->Q_angle);
    kalman->P[0][1] -= dt * kalman->P[1][1];
    kalman->P[1][0] -= dt * kalman->P[1][1];
    kalman->P[1][1] += kalman->Q_bias * dt;

    // Step 2: Update
    float S = kalman->P[0][0] + kalman->R_measure; // Estimate error
    float K[2]; // Kalman gain
    K[0] = kalman->P[0][0] / S;
    K[1] = kalman->P[1][0] / S;

    float y = newAngle - kalman->angle; // Angle difference
    kalman->angle += K[0] * y;
    kalman->bias += K[1] * y;

    // Update error covariance
    float P00_temp = kalman->P[0][0];
    float P01_temp = kalman->P[0][1];

    kalman->P[0][0] -= K[0] * P00_temp;
    kalman->P[0][1] -= K[0] * P01_temp;
    kalman->P[1][0] -= K[1] * P00_temp;
    kalman->P[1][1] -= K[1] * P01_temp;

    return kalman->angle;
}

void Kalman_SetAngle(Kalman *kalman, float angle) {
    kalman->angle = angle;
}

float Kalman_GetRate(Kalman *kalman) {
    return kalman->rate;
}

void Kalman_SetQangle(Kalman *kalman, float Q_angle) {
    kalman->Q_angle = Q_angle;
}

void Kalman_SetQbias(Kalman *kalman, float Q_bias) {
    kalman->Q_bias = Q_bias;
}

void Kalman_SetRmeasure(Kalman *kalman, float R_measure) {
    kalman->R_measure = R_measure;
}

float Kalman_GetQangle(Kalman *kalman) {
    return kalman->Q_angle;
}

float Kalman_GetQbias(Kalman *kalman) {
    return kalman->Q_bias;
}

float Kalman_GetRmeasure(Kalman *kalman) {
    return kalman->R_measure;
}
