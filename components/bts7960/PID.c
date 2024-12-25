#include "pid.h"

// Khởi tạo đối tượng PID với các tham số mặc định
PID_Controller pid;

/**
 * @brief Hàm khởi tạo thông số cho bộ điều khiển PID
 *
 * @param kp Hệ số tỉ lệ (Proportional) của PID
 * @param kd Hệ số đạo hàm (Derivative) của PID
 * @param dt Chu kỳ lấy mẫu (thời gian giữa các lần cập nhật PID)
 * @param curpoint Giá trị hiện tại (giá trị đầu vào ban đầu của PID)
 */
void pid_init(float kp, float kd, float dt, float curpoint) 
{
    pid.kp = kp;                   // Gán hệ số P
    pid.kd = kd;                   // Gán hệ số D
    pid.dt = dt;                   // Gán chu kỳ lấy mẫu
    pid.curpoint = curpoint;       // Gán giá trị hiện tại
    pid.setpoint = curpoint;       // Giá trị mục tiêu ban đầu bằng giá trị hiện tại
    pid.previous_error = 0.0;      // Khởi tạo sai số trước đó bằng 0
}

/**
 * @brief Đặt giá trị mục tiêu (setpoint) cho bộ điều khiển PID
 *
 * @param setpoint Giá trị mục tiêu mới
 */
void pid_set_target_value(float setpoint) 
{
    /**
     * Truyền giá trị setpoint vào đây
     * (Giá trị mà hệ thống mong muốn để điều khiển đếnđến)
     */
    pid.setpoint = setpoint;       // Cập nhật giá trị mục tiêu
}

/**
 * @brief Hàm tính toán đầu ra của PID
 *
 * @param pid Con trỏ đến đối tượng PID_Controller
 * @return Giá trị đầu ra của PID
 */
float pid_compute(PID_Controller *pid) 
{
    float output = 0.0;            // Giá trị đầu ra ban đầu
    float error = pid->setpoint - pid->curpoint; // Sai số giữa giá trị mục tiêu và giá trị hiện tại

    // Tính đạo hàm của sai số
    float derivative_error = (error - pid->previous_error) / pid->dt;

    // Tính toán đầu ra dựa trên thuật toán PD
    output = (pid->kp * error) + (pid->kd * derivative_error);

    // Cập nhật sai số trước đó
    pid->previous_error = error;

    return output;                 // Trả về giá trị đầu ra của PID
}

/**
 * @brief Hàm điều khiển xi lanh sử dụng thuật toán PID
 *
 * @param current_position Vị trí hiện tại của xi lanh
 * @return Tốc độ PWM để điều khiển động cơ
 */
int16_t control_cylinder_with_pid(float current_position) 
{
    pid.curpoint = current_position;        // Cập nhật giá trị hiện tại cho PID
    float pid_output = pid_compute(&pid);   // Tính toán đầu ra PID

    float speed = 0;                        // Biến tốc độ điều khiển PWM




/**
 * Cần đưa giá trị đầu ra của hàm tính toán PID vào đây
 */
    if (pid_output > 0.1f) 
    {
        // Nếu đầu ra PID > 0, di chuyển xi lanh lên
        speed = 255;                        // Giới hạn tốc độ tối đa là 255
        lift_up(speed);                     // Gọi hàm điều khiển xi lanh lên
    } 
    else if (pid_output < -0.1f) 
    {
        // Nếu đầu ra PID < 0, di chuyển xi lanh xuống
        speed = 255;                        // Giới hạn tốc độ tối đa là 255
        lower_down(speed);                  // Gọi hàm điều khiển xi lanh xuống
    } 
    else 
    {
        // Nếu đầu ra PID xấp xỉ 0, dừng xi lanh
        stop_cylinder();                    // Gọi hàm dừng xi lanh
    }
    
    return speed;                           // Trả về tốc độ PWM
}
