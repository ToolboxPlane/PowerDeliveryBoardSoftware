//
// Created by paul on 28.03.18.
//

#ifndef POWERDISTRIBUTIONBOARDSOFTWARE_IO_H
#define POWERDISTRIBUTIONBOARDSOFTWARE_IO_H

#include <stdbool.h>

/*
 * Alert V+: PC0
 * Alert 5V: PC1
 * Alert 3V3: PC2
 * Alert Temp: PD2
 */
#define ALERT_VCC ((PINC & (1 << 0))?0:1)
#define ALERT_5V ((PINC & (1 << 1))?0:1)
#define ALERT_3V3 ((PINC & (1 << 2))?0:1)
#define ALERT_TEMP ((PIND & (1 << 2))?0:1)

void set_reset(bool reset);

#endif //POWERDISTRIBUTIONBOARDSOFTWARE_IO_H
