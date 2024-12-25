

#include <esp_log.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include "I2Cdev.h"

#define I2C_NUM I2C_NUM_0

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); /*assert(0 && #x);*/} } while(0);


/** Initialize I2C0
 */
// void I2Cdev_initialize() {

// }

void I2Cdev_enable(bool isEnabled) {
  
}

/** Default timeout value for read operations.
 */
uint16_t I2Cdev_readTimeout = I2CDEV_DEFAULT_READ_TIMEOUT;


int8_t I2Cdev_readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout) {

	uint8_t b;
    uint8_t count = I2Cdev_readByte(devAddr, regAddr, &b, timeout);
    *data = b & (1 << bitNum);
    return count;
}


int8_t I2Cdev_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout) {
    uint8_t count, b;
    if ((count = I2Cdev_readByte(devAddr, regAddr, &b, timeout)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        *data = b;
    }
    return count;
}

int8_t I2Cdev_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout) {
    return I2Cdev_readBytes(devAddr, regAddr, 1, data, timeout);
}

int8_t I2Cdev_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout) {
	i2c_cmd_handle_t cmd;
	I2Cdev_SelectRegister(devAddr, regAddr);

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_READ, 1));

	if(length>1)
		ESP_ERROR_CHECK(i2c_master_read(cmd, data, length-1, I2C_MASTER_ACK));

	ESP_ERROR_CHECK(i2c_master_read_byte(cmd, data+length-1, I2C_MASTER_NACK));

	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
	i2c_cmd_link_delete(cmd);

	return length;
}

bool I2Cdev_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data){

	uint8_t data1[] = {(uint8_t)(data>>8), (uint8_t)(data & 0xff)};
	I2Cdev_writeBytes(devAddr, regAddr, 2, data1);
	return true;
}

void I2Cdev_SelectRegister(uint8_t devAddr, uint8_t regAddr){
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, regAddr, 1));
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
	i2c_cmd_link_delete(cmd);
}


bool I2Cdev_writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    I2Cdev_readByte(devAddr, regAddr, &b, I2Cdev_readTimeout);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return I2Cdev_writeByte(devAddr, regAddr, b);
}

bool I2Cdev_writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {

    uint8_t b = 0;
    if (I2Cdev_readByte(devAddr, regAddr, &b, I2Cdev_readTimeout) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return I2Cdev_writeByte(devAddr, regAddr, b);
    } else {
        return false;
    }
}

bool I2Cdev_writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data) {
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, regAddr, 1));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data, 1));
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
	i2c_cmd_link_delete(cmd);

	return true;
}

bool I2Cdev_writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data){
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(cmd));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, 1));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, regAddr, 1));
	ESP_ERROR_CHECK(i2c_master_write(cmd, data, length-1, 0));
	ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data[length-1], 1));
	ESP_ERROR_CHECK(i2c_master_stop(cmd));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM, cmd, 1000/portTICK_PERIOD_MS));
	i2c_cmd_link_delete(cmd);
	return true;
}


int8_t I2Cdev_readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout){
	uint8_t msb[2] = {0,0};
	I2Cdev_readBytes(devAddr, regAddr, 2, msb, I2Cdev_readTimeout);
	*data = (int16_t)((msb[0] << 8) | msb[1]);
	return 0;
}