/**
 * @file output.c
 * @author paul
 * @date 18.04.19
 * @brief output @TODO
 */

#include <avr/io.h>
#include "output.h"
#include "../HAL/pwm16bit.h"

void output_init(void) {
    DDRL = 0xFF; // All as outputs
}

void output_led(uint8_t led, led_t state) {
    switch (state) {
        case on:
            PORTL |= (1 << led);
            break;
        case off:
            PORTL &= ~(1 << led);
            break;
        case toggle:
            PORTL ^= (1 << led);
            break;
        default:
            break;
    }
}
