#ifndef PID_H
#define PID_H

#include <stdint.h>
#include <math.h>

#include "BTS7960.h"
// Cấu trúc PID
typedef struct
{                         
    float kp;             // Hệ số P (Proportional) 
    float kd;             // Hệ số D (Derivative)
    float setpoint;       // Giá trị mục tiêu, tại thời điểm khởi động thì nên viết code theo kiểu sẽ init cho phânf giá trị hiện tại nghe sẽ hợp lý
                         
    float curpoint;      
    float previous_error; // Sai số ở lần đo trước
    float dt;             // Chu kì lấy mẫu
} PID_Controller;

/**
 * @brief Khởi tạo cấu trúc PID
 *
 * @param kp Hệ số P
 * @param kd Hệ số D
 * @param curpoint Giá trị hiện tại cho PID
 * @return PID_Controller Cấu trúc PID được khởi tạo
 */
void pid_init(float kp, float kd, float dt,  float curpoint);

void pid_set_target_value(float setpoint);

/**
 * @brief Tính toán đầu ra của PID
 *
 * @param pid Con trỏ đến cấu trúc PID
 * @return float Giá trị điều khiển đầu ra
 */
float pid_compute(PID_Controller *pid);

int16_t control_cylinder_with_pid(float current_position);


#endif // PID_H
