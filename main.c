#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "Drivers/ltc2946.h"

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE

int main() {
    // Disable the interrupts
    cli();
    ltc2946_init(LTC_VCC_ADDR,  0, 0xFFFFFFFF, 14000, 20000, 0, 1000);
    ltc2946_init(LTC_5V_ADDR,  0, 0xFFFFFFFF, 4500, 5500, 0, 500);
    // Enable the interrupts
    sei();
    wdt_enable(WDTO_120MS);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        wdt_reset();
    }
#pragma clang diagnostic pop
}