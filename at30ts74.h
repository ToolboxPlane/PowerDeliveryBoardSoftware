//
// Created by paul on 28.03.18.
//

#ifndef POWERDISTRIBUTIONBOARDSOFTWARE_AT30TS74_H
#define POWERDISTRIBUTIONBOARDSOFTWARE_AT30TS74_H

#include <avr/io.h>

void at30ts74_setup(uint8_t addr, int8_t minTemp, int8_t maxTemp);
int8_t at30ts74_read(uint8_t addr);

#endif //POWERDISTRIBUTIONBOARDSOFTWARE_AT30TS74_H
