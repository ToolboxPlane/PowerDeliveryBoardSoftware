#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "Drivers/ltc2946.h"
#include "Util/communication.h"

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE

int main() {
    // Disable the interrupts
    cli();
    ltc2946_init(LTC_VCC_ADDR,  0, 0xFFFFFFFF, 14000, 20000, 0, 1000);
    ltc2946_init(LTC_5V_ADDR,  0, 0xFFFFFFFF, 4500, 5500, 0, 500);
    communication_init();
    // Enable the interrupts
    sei();
    wdt_enable(WDTO_120MS);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    uint8_t status_5V, status_vcc;
    ltc_result_t meas_5V, meas_vcc;
    while (true) {
        status_vcc = ltc2946_status(LTC_VCC_ADDR);
        status_5V = ltc2946_status(LTC_5V_ADDR);
        ltc2946_read(LTC_VCC_ADDR, &meas_vcc);
        ltc2946_read(LTC_5V_ADDR, &meas_5V);

        communication_send_data(status_5V | status_vcc,
                &meas_vcc, &meas_5V);

        wdt_reset();
    }
#pragma clang diagnostic pop
}