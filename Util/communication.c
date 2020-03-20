/**
 * @file communication.cpp
 * @author paul
 * @date 24.04.19
 * @brief communication @TODO
 */

#include "communication.h"
#include "../HAL/uart.h"
#include "../Drivers/rc_lib/rc_lib.h"

#define TRANSMITTER_ID 74

void communication_init(void) {
    uart_init(0, 115200, NONE, 1, 0);
}

void communication_send_data(uint8_t status, const ltc_result_t *meas_vcc, const ltc_result_t *meas_5v) {
    rc_lib_package_t pkg;
    pkg.resolution = 256;
    pkg.channel_count = 8;
    pkg.mesh = false;

    pkg.channel_data[0] = status;
    pkg.channel_data[1] = meas_vcc->voltageMilliVolts / 128;
    pkg.channel_data[2] = meas_vcc->currentMilliAmperes / 256;
    pkg.channel_data[3] = meas_5v->voltageMilliVolts / 32;
    pkg.channel_data[4] = meas_5v->currentMilliAmperes / 64;
    pkg.channel_data[5] = 0;
    pkg.channel_data[6] = 0;
    pkg.channel_data[7] = 0;

    rc_lib_transmitter_id = TRANSMITTER_ID;
    uint8_t len = rc_lib_encode(&pkg);
    uart_send_buf(0, pkg.buffer, len);
}
