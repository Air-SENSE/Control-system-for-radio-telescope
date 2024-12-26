#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "QMC5883L.h"
#include "mpu6050.h"
#include "kalman.h"
#include "freertos/FreeRTOS.h"


SemaphoreHandle_t mutex; // Mutex để bảo vệ truy cập vào biến gz
volatile double shared_gz = 0; // Biến chia sẻ giữa hai task


void task_mpu6050(void *pvParameters) {
	MPU6050 mpu;
	MPU6050_init(&mpu, MPU6050_DEFAULT_ADDRESS);

	uint8_t devid = MPU6050_getDeviceID(&mpu);
	ESP_LOGI("MPU6050_task", "devid=0x%x", devid);

	uint8_t rate = MPU6050_getRate(&mpu);
    ESP_LOGI("MPU6050_task", "getRate()=%d", rate);
	if (rate != 0) MPU6050_setRate(&mpu, 0);

	uint8_t ExternalFrameSync = MPU6050_getExternalFrameSync(&mpu);
	ESP_LOGI("MPU6050_task", "getExternalFrameSync()=%d", ExternalFrameSync);
	if (ExternalFrameSync != 0) MPU6050_setExternalFrameSync(&mpu, 0);;

	uint8_t DLPFMode = MPU6050_getDLPFMode(&mpu);
	ESP_LOGI("MPU6050_task", "getDLPFMode()=%d", DLPFMode);
	if (DLPFMode != 6) MPU6050_setDLPFMode(&mpu, 6);

	uint8_t FullScaleAccelRange = MPU6050_getFullScaleGyroRange(&mpu);
	ESP_LOGI("MPU6050_task", "getFullScaleAccelRange()=%d", FullScaleAccelRange);
	if (FullScaleAccelRange != 0) MPU6050_setFullScaleGyroRange(&mpu, 0); 
	float accel_sensitivity = 16384.0; // g

	uint8_t FullScaleGyroRange = MPU6050_getFullScaleAccelRange(&mpu);
	ESP_LOGI("MPU6050_task", "getFullScaleGyroRange()=%d", FullScaleGyroRange);
	if (FullScaleGyroRange != 0) MPU6050_setFullScaleAccelRange(&mpu, 0);
	float gyro_sensitivity = 131.0; // Deg/Sec

	MPU6050_setI2CBypassEnabled(&mpu, true);

	double ax, ay, az;
    double gx, gy, gz;
    float Elevation;
    // float prevElevation = 0; // Lưu trữ giá trị Elevation trước đó

	//kalman filter
	Kalman kalman;
	Kalman_Init(&kalman);
	float dt = 0.1; // Assuming 100 ms delay between readings

    while (1) {
        MPU6050_getMotion6(&mpu, &ax, &ay, &az, &gx, &gy, &gz, accel_sensitivity, gyro_sensitivity);
		// Khóa mutex và cập nhật shared_gz
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {
            shared_gz = gz; // Cập nhật giá trị gz chia sẻ
            xSemaphoreGive(mutex); // Mở khóa mutex
        }
        Elevation = MPU6050_getElevation(&mpu, ax, ay, az);
		float smoothedElevation = Kalman_GetAngle(&kalman, Elevation, gy, dt);
		printf(" Elevation = %f\n", smoothedElevation);
        // Kiểm tra thay đổi góc
        // if (fabs(Elevation - prevElevation) >= 5.0) {
        //     printf(" Elevation = %f\n", Elevation);
        //     prevElevation = Elevation; // Cập nhật giá trị trước đó
        // }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void task_qmc5883l(void *pvParameters)
{
	QMC5883L qmc;

	QMC5883L_Init(&qmc, QMC5883_ADDR);
	QMC5883L_Begin(&qmc);
	QMC5883L_SetConfig(&qmc);
	// Calibration calibration = {
    //     .axes = {
    //         {-1008, 738},   // Trục X 
    //         {26, 2273},   // Trục Y
    //         {1363, 2230}    // Trục Z
    //     }
    // };
	// QMC5883L_SetCalibration(&qmc, &calibration);

	float Azimuth;
    // float prevAzimuth = 0; // Lưu trữ giá trị Azimuth trước đó
	// float RawDataX, prevRawDataX = QMC5883L_rawDataX(&qmc);
	// float RawDataY, prevRawDataY = QMC5883L_rawDataY(&qmc);
	// float RawDataZ, prevRawDataZ = QMC5883L_rawDataZ(&qmc);

	//kalman filter
	Kalman kalman;
	Kalman_Init(&kalman);
	float dt = 0.1; // Assuming 100 ms delay between readings

    while (1) {
        Azimuth = QMC5883L_AzimuthZUp(&qmc);
		double gz = 0;
		if (xSemaphoreTake(mutex, portMAX_DELAY)) {
            gz = shared_gz; // Đọc giá trị gz được chia sẻ
            xSemaphoreGive(mutex); // Mở khóa mutex
        }
		float smoothedAzimuth = Kalman_GetAngle(&kalman, Azimuth, gz, dt);
		printf(" Azimuth = %f\n", smoothedAzimuth);
        // Kiểm tra thay đổi góc
        // if (fabs(Azimuth - prevAzimuth) >= 5.0) {
        //     printf(" Azimuth = %f\n", Azimuth);
        //     prevAzimuth = Azimuth; // Cập nhật giá trị trước đó
        // }

		//dò max, min raw data
		// RawDataX = QMC5883L_rawDataX(&qmc);
		// RawDataY = QMC5883L_rawDataY(&qmc);
		// RawDataZ = QMC5883L_rawDataZ(&qmc);
		// if(RawDataX <= prevRawDataX)
		// {
			// printf("minRawDataX = %f\n", RawDataX);
		// 	prevRawDataX = RawDataX;
		// } 
		// if(RawDataY <= prevRawDataY)
		// {
			// printf("minRawDataY = %f\n", RawDataY);
		// 	prevRawDataY = RawDataY;
		// } 
		// if(RawDataZ <= prevRawDataZ)
		// {
			// printf("minRawDataZ = %f\n", RawDataZ);
		// 	prevRawDataZ = RawDataZ;
		// } 

		// printf("RawDataX = %d\n", QMC5883L_rawDataX(&qmc));
		// printf("RawDataY = %d\n", QMC5883L_rawDataY(&qmc));
		// printf("RawDataZ = %d\n", QMC5883L_rawDataZ(&qmc));
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void start_i2c(void) {
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = (gpio_num_t)CONFIG_GPIO_SDA;
	conf.scl_io_num = (gpio_num_t)CONFIG_GPIO_SCL;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 400000;
	conf.clk_flags = 0;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

void app_main(void)
{
	start_i2c();
	// Tạo mutex
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        ESP_LOGE("app_main", "Failed to create mutex!");
        return;
    }
	xTaskCreate(task_mpu6050, "MPU6050_task", 1024*8, NULL, 1, NULL);
	xTaskCreate(task_qmc5883l, "QMC5883L_task", 1024*8, NULL, 1, NULL);

	vTaskDelay(100);
}
