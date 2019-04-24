/**
 * @file communication.hpp
 * @author paul
 * @date 24.04.19
 * @brief communication @TODO
 */

#include "../Drivers/ltc2946.h"

#ifndef POWERDELIVERYBOARDSOFTWARE_COMMUNICATION_H
#define POWERDELIVERYBOARDSOFTWARE_COMMUNICATION_H

void communication_init(void);
void communication_send_data(uint8_t status, const ltc_result_t *meas_vcc, const ltc_result_t *meas_5v);

#endif //POWERDELIVERYBOARDSOFTWARE_COMMUNICATION_H
