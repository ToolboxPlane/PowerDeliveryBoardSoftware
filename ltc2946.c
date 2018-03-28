//
// Created by paul on 28.03.18.
//

#include "ltc2946.h"

void ltc2946_setup(uint8_t deviceAddr, uint32_t minPower, uint32_t maxPower, uint16_t minVoltage, uint16_t maxVoltage,
                   uint16_t minCurrent,
                   uint16_t maxCurrent) {
    // Values from real units to Val/LSB
    minVoltage /=  25;
    maxVoltage /= 25;
    minCurrent /= 12;
    maxCurrent /= 12;
    minPower /= 312;
    maxPower /= 312;

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x0E); // MaxPower
    i2c_write((unsigned char)(maxPower >> 16));
    i2c_write((unsigned char)(maxPower >> 8));
    i2c_write((unsigned char)(maxPower));

    i2c_write((unsigned char) (minPower >> 16));
    i2c_write((unsigned char) (minPower >> 8));
    i2c_write((unsigned char) minPower);
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x24); // Max Voltage
    i2c_write((unsigned char) (maxVoltage >> 4));
    i2c_write((unsigned char) (maxVoltage << 4));

    i2c_write((unsigned char) (minVoltage >> 4));
    i2c_write((unsigned char) (minVoltage << 4));
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x1A); // Max Current
    i2c_write((unsigned char) (maxCurrent >> 4));
    i2c_write((unsigned char) (maxCurrent << 4));

    i2c_write((unsigned char) (minCurrent >> 4));
    i2c_write((unsigned char) (minCurrent << 4));
    i2c_stop();


    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x01); // CTRLB
    i2c_write(0b10100000); // Alert clear on read, fault clear on read
    i2c_write(0b00000100); // Enable all alerts @TODO really enable all
    i2c_stop();
}

void ltc2946_read(uint8_t deviceAddr, ltc_result_t *result) {
    uint32_t tmp;

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x1E);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp= (uint16_t)i2c_readAck() << 4;
    tmp |= i2c_readNak() >> 4;
    result->voltageMilliVolts = tmp*25;
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x14);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp= (uint16_t)i2c_readAck() << 4;
    tmp |= i2c_readNak() >> 4;
    result->currentMilliAmperes = (uint16_t)(tmp*12.5);
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x3C);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp = (uint32_t)i2c_readAck() << 24;
    tmp |= (uint32_t)i2c_readAck() << 16;
    tmp |= (uint32_t)i2c_readAck() << 8;
    tmp |= (uint32_t)i2c_readNak();
    result->energyMilliJoules = (uint16_t)(tmp*335.78);
    i2c_stop();
}

