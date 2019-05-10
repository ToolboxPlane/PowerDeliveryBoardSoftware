#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/delay.h>
#include "Drivers/ltc2946.h"
#include "Util/communication.h"
#include "Util/output.h"

#define LTC_VCC_ADDR 0x6F
#define LTC_5V_ADDR 0x67

int main() {
    // Disable the interrupts
    cli();
    output_init();
    output_led(0, on);
    output_led(7, on);
    output_led(1, MCUSR & (1 << WDRF) ? off : on); // Watchdog
    output_led(2, MCUSR & (1 << BORF) ? off : on); // Brownout
    MCUSR = 0;
    communication_init();
    // Enable the interrupts
    sei();

    wdt_enable(WDTO_30MS);
    ltc2946_init(LTC_VCC_ADDR,  0, 0xFFFFFFFF, 14000, 20000, 0, 1000);
    ltc2946_init(LTC_5V_ADDR,  0, 0xFFFFFFFF, 4500, 5500, 0, 500);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    uint8_t status_5V, status_vcc;
    ltc_result_t meas_5V, meas_vcc;
    while (true) {
        wdt_reset();
        output_led(0, toggle);

        status_vcc = ltc2946_status(LTC_VCC_ADDR);
        status_5V = ltc2946_status(LTC_5V_ADDR);

        if(ltc2946_read(LTC_VCC_ADDR, &meas_vcc)) {
            output_led(3, on);
        } else {
            output_led(3, off);
        }
        if(ltc2946_read(LTC_5V_ADDR, &meas_5V)) {
            output_led(4, on);
        } else {
            output_led(4, off);
        }

        output_led(5, status_vcc == 0 ? on : off);
        output_led(6, status_5V == 0 ? on : off);

        communication_send_data(status_5V | status_vcc,
                &meas_vcc, &meas_5V);
        _delay_ms(10);

    }
#pragma clang diagnostic pop
}