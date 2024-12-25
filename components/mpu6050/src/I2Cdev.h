

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include <driver/i2c.h>
#include <stdint.h>
#include <stdbool.h>

#define I2C_SDA_PORT gpioPortA
#define I2C_SDA_PIN 21
#define I2C_SDA_MODE gpioModeWiredAnd
#define I2C_SDA_DOUT 21

#define I2C_SCL_PORT gpioPortA
#define I2C_SCL_PIN 22 
#define I2C_SCL_MODE gpioModeWiredAnd
#define I2C_SCL_DOUT 21

#define I2CDEV_DEFAULT_READ_TIMEOUT 1000

    // void I2Cdev_initialize();
    void I2Cdev_enable(bool isEnabled);

    int8_t I2Cdev_readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout);
     
    int8_t I2Cdev_readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout);
        
    int8_t I2Cdev_readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout);
    int8_t I2Cdev_readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout);
    int8_t I2Cdev_readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);
        

    bool I2Cdev_writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
        
    bool I2Cdev_writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
       
    bool I2Cdev_writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
    bool I2Cdev_writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
    bool I2Cdev_writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
        
    void I2Cdev_SelectRegister(uint8_t devAddr, uint8_t regAddr);

    extern uint16_t I2Cdev_readTimeout;

#endif /* _I2CDEV_H_ */