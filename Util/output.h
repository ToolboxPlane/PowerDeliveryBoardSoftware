/**
 * @file output.h
 * @author paul
 * @date 18.04.19
 * @brief output @TODO
 */

#ifndef POWERDELIVERYBOARDSOFTWARE_OUTPUT_H
#define POWERDELIVERYBOARDSOFTWARE_OUTPUT_H

#include <stdbool.h>

typedef enum {
    on, off, toggle, keep
} led_t;

void output_init(void);
void output_led(uint8_t led, led_t state);

#endif //POWERDELIVERYBOARDSOFTWARE_OUTPUT_H
