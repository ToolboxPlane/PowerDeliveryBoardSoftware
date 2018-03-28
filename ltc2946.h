//
// Created by paul on 28.03.18.
//

#ifndef POWERDISTRIBUTIONBOARDSOFTWARE_LTC2946_H
#define POWERDISTRIBUTIONBOARDSOFTWARE_LTC2946_H

#include <avr/io.h>
#include "i2c/i2cmaster.h"

/*
 * Values / LSB:
 * Current: 12.5mA/LSB
 * Voltage: 25mv/LSB
 * Power: 312.5µW/LSB
 * Charge: 3279.086μC/LSB
 * Energy: 335.78mJ/LSB
 */

typedef struct {
    uint16_t voltageMilliVolts;
    uint16_t currentMilliAmperes;
    uint32_t energyMilliJoules;
} ltc_result_t;

void ltc2946_setup(uint8_t deviceAddr, uint32_t minPower, uint32_t maxPower, uint16_t minVoltage, uint16_t maxVoltage,
                   uint16_t minCurrent, uint16_t maxCurrent);
void ltc2946_read(uint8_t deviceAddr, ltc_result_t *result);

#endif //POWERDISTRIBUTIONBOARDSOFTWARE_LTC2946_H
