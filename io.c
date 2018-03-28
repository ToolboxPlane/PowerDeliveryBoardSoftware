//
// Created by paul on 28.03.18.
//

#include <avr/io.h>
#include "io.h"

void set_reset(bool reset) {
    if(reset) {
        PORTD |= (1 << 6); // Reset -> Low
        PORTD &= ~(1 << 7);
    } else {
        PORTD &= ~(1 << 6); // Reset -> High
        PORTD |= (1 << 7);
    }
}
