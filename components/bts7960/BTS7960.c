#include "BTS7960.h"


/**
 * @brief Hàm khởi tạo PWM
 * 
 * Thiết lập các kênh PWM để điều khiển BTS7960 ở hai chiều
 * (nâng và hạ) qua RPWM và LPWM.
 */
void pwm_init() {
    gpio_config_t io_conf = {
        /**
         *  Các chân điều khiển
         */
        .pin_bit_mask = (1ULL<<R_IS_GPIO) | (1ULL<<R_EN_GPIO) | (1ULL<<R_PWM_PIN) |
                        (1ULL<<L_IS_GPIO) | (1ULL<<L_EN_GPIO) | (1ULL<<L_PWM_PIN),
        
        .mode = GPIO_MODE_OUTPUT,              /** Thiết lập chân ở chế độ output */
        .intr_type = GPIO_PIN_INTR_DISABLE,    /** Không sử dụng ngắt ở chân này */
        .pull_down_en = GPIO_PULLDOWN_DISABLE, /** Không bật chế độ kéo xuốngxuống */
        .pull_up_en = GPIO_PULLUP_DISABLE,     /** Không bật chế độ kéo lên */
    };
    /**
     * Áp dụng cấu hình cho GPIO
     */
    gpio_config(&io_conf);

    /**
     * Thiết lập trạng thái ban đầu cho các chân điều khiển
     */
    gpio_set_level(R_IS_GPIO, 0);                // Chân điều hướng phải ở mức thấp (0)
    gpio_set_level(L_IS_GPIO, 0);                // Chân điều hướng trái ở mức thấp (0)
    gpio_set_level(R_EN_GPIO, 1);                // Kích hoạt chân Enable cho xi lanh quay phải
    gpio_set_level(L_EN_GPIO, 1);                // Kích hoạt chân Enable cho xi lanh quay trái
 
    /** Cấu hình bộ đếm thời gian (timer) cho PWM */
    ledc_timer_config_t ledc_timer = {          
        .speed_mode = LEDC_LOW_SPEED_MODE,       // Chế độ tốc độ thấp
        .duty_resolution = LEDC_TIMER_8_BIT,     // Độ phân giải 8 bit
        .timer_num = LEDC_TIMER_0,               // Sử dụng timer 0
        .freq_hz = 5000,                         // Tần số PWM là 5000hz
        .clk_cfg = LEDC_AUTO_CLK,                // Dùng xung clock tự động
    };
    /**
     *  Áp dụng cấu hình cho timer PWM
    */

    ledc_timer_config(&ledc_timer);
    /** 
    Cấu hình kênh PWM cho xi lanh phải 
    */
    ledc_channel_config_t pwm_channel = {
        .gpio_num = R_PWM_PIN,                   // Chân GPIO được sử dụng cho PWM của xi lanh phải
        .speed_mode = LEDC_LOW_SPEED_MODE,       // Chế độ tốc độ thấp
        .channel = R_PWM_CHANNEL,                // Kênh PWM dành cho xi lanh phải
        .intr_type = LEDC_INTR_DISABLE,          // Không sử dụng ngắt cho kênh PWM
        .timer_sel = LEDC_TIMER_0,               // Sử dung timer 0
        .duty = 0,                               // Đặt mức duty cycle ban đầu là 0
        .hpoint = 0,                             // Không có điểm hpoint
    };
    ledc_channel_config(&pwm_channel);  /** Áp dụng cấu hình cho kênh PWM xi lanh phải */
     
    /**
    Cấu hình kênh PWM cho xi lanh quay trái 
     */
    pwm_channel.gpio_num = L_PWM_PIN;            // Chân GPIO được sử dụng cho PWM của xi lanh trái
    pwm_channel.channel = L_PWM_CHANNEL;         // Kênh PWM dành cho xi lanh trái
    ledc_channel_config(&pwm_channel);           // Áp dụng cấu hình cho kênh PWM xi lanh trái
}

/**
 * @brief Hàm khởi tạo chân điều khiển
 * 
 * Đặt REN và LEN ở chế độ OUTPUT và thiết lập mức logic ban đầu.
 */
/**
 * @brief Nâng xi lanh lên với tốc độ chỉ định.
 * 
 * Hàm này điều khiển xi lanh nâng lên bằng cách thiết lập 
 * mức duty cycle PWM cho kênh điều khiển xi lanh phải 
 * và đặt duty cycle về 0 cho xi lanh trái.
 * 
 * @param speed Giá trị tốc độ nâng (0-255) tương ứng với duty cycle PWM.
 */
void lift_up(uint8_t speed) {
    // Thiết lập mức duty cycle PWM cho kênh điều khiển xi lanh phải
    ledc_set_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL, speed);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL);

    // Đặt mức duty cycle PWM về 0 cho kênh điều khiển xi lanh trái
    ledc_set_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL, 0);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL);
}


/**
 * @brief Hạ xi lanh xuống với tốc độ chỉ định.
 * 
 * Hàm này điều khiển xi lanh hạ xuống bằng cách thiết lập 
 * mức duty cycle PWM cho kênh điều khiển xi lanh trái 
 * và đặt duty cycle về 0 cho xi lanh phải.
 * 
 * @param speed Giá trị tốc độ hạ (0-255) tương ứng với duty cycle PWM.
 */
void lower_down(uint8_t speed) {
    // Đặt mức duty cycle PWM về 0 cho kênh điều khiển xi lanh phải
    ledc_set_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL, 0);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL);

    // Thiết lập mức duty cycle PWM cho kênh điều khiển xi lanh trái
    ledc_set_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL, speed);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL);
}


/**
 * @brief Dừng hoạt động của xi lanh.
 * 
 * Hàm này dừng cả hai xi lanh (trái và phải) bằng cách đặt 
 * mức duty cycle PWM về 0 cho cả hai kênh điều khiển.
 * 
 * Không có chuyển động nào xảy ra khi hàm này được gọi.
 */
void stop_cylinder() {
    // Đặt mức duty cycle PWM về 0 cho kênh điều khiển xi lanh phải
    ledc_set_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL, 0);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, R_PWM_CHANNEL);

    // Đặt mức duty cycle PWM về 0 cho kênh điều khiển xi lanh trái
    ledc_set_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL, 0);
    // Cập nhật giá trị duty cycle để áp dụng thay đổi
    ledc_update_duty(LEDC_LOW_SPEED_MODE, L_PWM_CHANNEL);
}

