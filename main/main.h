#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "mpu6050.h"
#include "I2Cdev.h"
#include "PID.h"
#include "BTS7960.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>
#include <hal/gpio_types.h>

#define GPIO_SDA_PIN 21 // Nhập chân thực tế
#define GPIO_SCL_PIN 22

#endif 