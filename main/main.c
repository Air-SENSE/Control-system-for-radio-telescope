#include <main.h>

/**
 * @brief Task điều khiển xi lanh sử dụng cảm biến MPU6050 và thuật toán PID
 *
 * @param pvParameters Tham số truyền vào task (không sử dụng trong trường hợp này)
 */
void task_xylanh_mpu6050(void *pvParameters)
{
    // Khởi tạo PWM cho xi lanh
    pwm_init();

    // Khởi tạo cảm biến MPU6050 với địa chỉ mặc định
    MPU6050 mpu;
    MPU6050_init(&mpu, MPU6050_DEFAULT_ADDRESS);

    // Kiểm tra và cấu hình các thông số của MPU6050
    uint8_t devid = MPU6050_getDeviceID(&mpu);

    uint8_t rate = MPU6050_getRate(&mpu);
    if (rate != 0)
        MPU6050_setRate(&mpu, 0); // Thiết lập tần số lấy mẫu mặc định

    uint8_t ExternalFrameSync = MPU6050_getExternalFrameSync(&mpu);
    if (ExternalFrameSync != 0)
        MPU6050_setExternalFrameSync(&mpu, 0); // Tắt đồng bộ khung ngoài

    uint8_t DLPFMode = MPU6050_getDLPFMode(&mpu);
    if (DLPFMode != 6)
        MPU6050_setDLPFMode(&mpu, 6); // Thiết lập bộ lọc thông thấp (DLPF)

    uint8_t FullScaleAccelRange = MPU6050_getFullScaleGyroRange(&mpu);
    if (FullScaleAccelRange != 0)
        MPU6050_setFullScaleGyroRange(&mpu, 0); // Phạm vi con quay hồi chuyển ±250°/s
    float accel_sensitivity = 16384.0; // Độ nhạy gia tốc (đơn vị g)

    uint8_t FullScaleGyroRange = MPU6050_getFullScaleAccelRange(&mpu);
    if (FullScaleGyroRange != 0)
        MPU6050_setFullScaleAccelRange(&mpu, 0); // Phạm vi gia tốc ±2g
    float gyro_sensitivity = 131.0; // Độ nhạy con quay hồi chuyển (đơn vị °/s)

    MPU6050_setI2CBypassEnabled(&mpu, true); // Kích hoạt chế độ bypass I2C

    // Khai báo biến lưu dữ liệu gia tốc, con quay và góc nghiêng
    double ax, ay, az;
    double gx, gy, gz;
    float Elevation;
    float prevElevation = 0; // Lưu giá trị Elevation trước đó để kiểm tra sự thay đổi

    while (1)
    {
        // Lấy dữ liệu từ cảm biến MPU6050
        MPU6050_getMotion6(&mpu, &ax, &ay, &az, &gx, &gy, &gz, accel_sensitivity, gyro_sensitivity);
        Elevation = MPU6050_getElevation(&mpu, ax, ay, az); // Tính toán góc nghiêng (Elevation)



        // Khởi tạo PID và thiết lập setpoint
        pid_init(2.0f, 0.5f, 0.0f, Elevation); // kp=2.0, kd=0.5, dt=0.0
        /**
         * Thông số PD chưa chuẩn
         */


        pid_set_target_value(0.0f); // Giá trị mục tiêu (setpoint) là 0.0
        /**
         * Hiện tại đang lấy giá trị set_point giả định là 0.00
         */


        // Tính toán tốc độ điều khiển dựa trên PID
        uint8_t speed = control_cylinder_with_pid(Elevation);
        /** 
         * 
         */


        // Kiểm tra sự thay đổi lớn trong góc nghiêng và in thông tin ra console
        if (fabs(Elevation - prevElevation) >= 5.0)
        {
            printf("Elevation = %f, speed = %d\n", Elevation, speed);
        }

        // Cập nhật giá trị Elevation trước đó
        prevElevation = Elevation;

        // Delay task trong 100ms
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Hàm khởi tạo giao tiếp I2C cho cảm biến MPU6050
 */
void start_i2c(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;             // Cấu hình chế độ I2C master
    conf.sda_io_num = GPIO_SDA_PIN;          // Gán chân SDA
    conf.scl_io_num = GPIO_SCL_PIN;          // Gán chân SCL
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE; // Kích hoạt pull-up cho chân SDA
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE; // Kích hoạt pull-up cho chân SCL
    conf.master.clk_speed = 400000;          // Tần số xung clock 400kHz
    conf.clk_flags = 0;                      // Không sử dụng cờ bổ sung
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf)); // Cấu hình thông số I2C
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0)); // Cài đặt driver I2C
}

/**
 * @brief Hàm chính của chương trình
 */
void app_main(void)
{
    // Khởi tạo I2C
    start_i2c();

    // Tạo một task để điều khiển xi lanh sử dụng MPU6050
    xTaskCreate((TaskFunction_t)task_xylanh_mpu6050, "[task_xylanh_mpu6050]", 1024 * 8, NULL, 1, NULL);

    // Xóa task main sau khi task điều khiển được tạo
    vTaskDelete(NULL);
}
