set(app_src mpu6050.c I2Cdev.c)
set(pre_req i2cdev driver log esp_idf_lib_helpers)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
