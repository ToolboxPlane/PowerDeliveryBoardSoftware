//
// Created by paul on 28.03.18.
//

#include "ltc2946.h"

static volatile bool _finished = false;
static volatile bool _success = false;

static void callback(bool success) {
    _success = success;
    _finished = true;
}

bool ltc2946_init(uint8_t deviceAddr, uint32_t minPower, uint32_t maxPower, uint16_t minVoltage, uint16_t maxVoltage,
                  uint16_t minCurrent,
                  uint16_t maxCurrent) {
    // Values from real units to Val/LSB
    minVoltage /= 25;
    maxVoltage /= 25;
    minCurrent /= 12;
    maxCurrent /= 12;
    minPower /= 312;
    maxPower /= 312;

    uint8_t maxMinPowerBuf[] = {0x0E, maxPower >> 16, maxPower >> 8, maxPower,
                                minPower >> 16, minPower >> 8, minPower};
    uint8_t maxMinVoltageBuf[] = {0x24, maxVoltage >> 4, maxVoltage << 4,
                                  minVoltage >> 4, minVoltage << 4};
    uint8_t maxMinCurrBuf[] = {0x1A, maxCurrent >> 4, maxCurrent << 4,
                               minCurrent >> 4, minCurrent << 4};

    i2c_init(I2C_CLOCK_100K);
    _finished = false;
    i2c_send_receive(deviceAddr, maxMinPowerBuf, sizeof(maxMinPowerBuf), 0, 0, &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    _finished = false;
    i2c_send_receive(deviceAddr, maxMinVoltageBuf, sizeof(maxMinVoltageBuf), 0, 0, &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    _finished = false;
    i2c_send_receive(deviceAddr, maxMinCurrBuf, sizeof(maxMinCurrBuf), 0, 0, &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }

    // Register CTRL-B:
    // Alert clear on read, fault clear on read.
    // Enable all alerts
    uint8_t ctrlbBuf[] = {0x01, 0b10100000, 0b11111111};
    _finished = false;
    i2c_send_receive(deviceAddr, ctrlbBuf, sizeof(ctrlbBuf), 0, 0, &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    return true;
}

bool ltc2946_read(uint8_t deviceAddr, ltc_result_t *result) {
    uint8_t voltageWriteBuf = 0x1E;
    uint8_t voltageReadBuf[2];
    _finished = false;
    i2c_send_receive(deviceAddr, &voltageWriteBuf, 1, voltageReadBuf, sizeof(voltageReadBuf), &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    result->voltageMilliVolts = ((uint16_t)voltageReadBuf[0] << 4 | voltageReadBuf[1] >> 4) * 25;

    uint8_t currWriteBuf = 0x14;
    uint8_t currReadBuf[2];
    _finished = false;
    i2c_send_receive(deviceAddr, &currWriteBuf, 1, currReadBuf, sizeof(currReadBuf), &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    result->currentMilliAmperes = ((uint16_t)currReadBuf[0] << 4 | currReadBuf[1] >> 4) * 12.5;

    uint8_t energyWriteBuf = 0x3E;
    uint8_t energyReadBuf[4];
    _finished = false;
    i2c_send_receive(deviceAddr, &energyWriteBuf, 1, energyReadBuf, sizeof(energyReadBuf), &callback);
    while (!_finished);
    if (!_success) {
        return false;
    }
    result->energyMilliJoules = ((uint32_t)energyReadBuf[0] << 24 | (uint32_t)energyReadBuf[1] << 16 |
                            (uint32_t)energyReadBuf[2] << 8 | energyReadBuf[3]) * 335.78;
    return true;
}

uint8_t ltc2946_status(uint8_t deviceAddr) {
    uint8_t tmp = 0x03;
    _finished = false;
    i2c_send_receive(deviceAddr, &tmp, 1, &tmp, 1, &callback);
    while (!_finished);
    return tmp;
}

uint8_t ltc2946_fault(uint8_t deviceAddr) {
    uint8_t tmp = 0x04;
    _finished = false;
    i2c_send_receive(deviceAddr, &tmp, 1, &tmp, 1, &callback);
    while (!_finished);
    return tmp;
}

uint8_t ltc2946_status2(uint8_t deviceAddr) {
    uint8_t tmp = 0x40;
    _finished = false;
    i2c_send_receive(deviceAddr, &tmp, 1, &tmp, 1, &callback);
    while (!_finished);
    return tmp;
}

