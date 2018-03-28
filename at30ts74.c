//
// Created by paul on 28.03.18.
//

#include "at30ts74.h"
#include "i2c/i2cmaster.h"

void at30ts74_setup(uint8_t addr, int8_t minTemp, int8_t maxTemp) {
    i2c_start(addr | I2C_WRITE);
    i2c_write(0x02); // T_Low
    i2c_write((unsigned char) minTemp);
    i2c_write(0);
    i2c_write((unsigned char) maxTemp);
    i2c_stop();
}

int8_t at30ts74_read(uint8_t addr) {
    i2c_start(addr | I2C_WRITE);
    i2c_write(0x00);
    i2c_rep_start(addr | I2C_READ);
    int8_t temp = i2c_readNak();
    i2c_stop();

    return temp;
}
