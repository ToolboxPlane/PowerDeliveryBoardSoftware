#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE

int main() {
    // Disable the interrupts
    cli();

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