#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>
#include <stdio.h>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/gpio.h"

// Định nghĩa các chân kết nối với module BTS7960

#define R_IS_GPIO 25                   // Chân điều khiển chiều dòng điện cho xi lanh phải
#define R_EN_GPIO 2                    // Chân kích hoạt (enable) xi lanh phải
#define R_PWM_CHANNEL LEDC_CHANNEL_0   // Kênh PWM cho xi lanh phải
#define R_PWM_PIN GPIO_NUM_18          // Chân đầu ra PWM cho xi lanh phải
 
#define L_IS_GPIO 26                   // Chân điều khiển chiều dòng điện cho xi lanh trái
#define L_EN_GPIO 4                    // Chân kích hoạt (enable) xi lanh trái
#define L_PWM_CHANNEL LEDC_CHANNEL_1   // Kênh PWM cho xi lanh trái
#define L_PWM_PIN GPIO_NUM_19          // Chân đầu ra PWM cho xi lanh trái

// Các hằng số cấu hình PWM

#define PWM_FREQUENCY 5000    // Tần số PWM (Hz), quyết định tốc độ điều khiển
#define PWM_RESOLUTION LEDC_TIMER_8_BIT  // Độ phân giải PWM (8 bit: giá trị từ 0 đến 255)
#define PWM_MAX_DUTY 255      // Giá trị duty cycle tối đa cho PWM (100%)

/**
 * @brief Khởi tạo PWM
 * 
 * Cấu hình các kênh và timer PWM để điều khiển BTS7960.
 */
void pwm_init(void);

/**
 * @brief Khởi tạo các chân điều khiển xi lanh
 * 
 * Thiết lập các chân điều khiển (REN, LEN) ở chế độ OUTPUT.
 */
void lift_up(uint8_t speed);

/**
 * @brief Hạ xi lanh
 * 
 * @param speed Giá trị tốc độ PWM (0-255)
 * 
 * Kích hoạt chiều hạ xi lanh với tốc độ điều chỉnh qua PWM.
 */
void lower_down(uint8_t speed);

/**
 * @brief Dừng xi lanh
 * 
 * Dừng cả hai chiều (nâng và hạ) và tắt tín hiệu PWM.
 */
void stop_cylinder(void);

#endif // MOTOR_CONTROL_H
