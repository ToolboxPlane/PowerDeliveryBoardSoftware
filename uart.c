//
// Created by paul on 27.03.18.
//

#include "uart.h"

void uart_init(uint32_t baud) {
    UBRR0 = (uint16_t)(F_CPU/(16.0f * baud) - 1); // Baudrate
    UCSR0A = 0; // Clear flags, no double speed, no multiprocessor communication mode
    UCSR0B = 1 << TXEN0; // Tx-Enabled
    UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00); // Synchronous Mode, 8-bit Words
}

void uart_send_buf(uint8_t *buf, uint8_t len) {
    for(uint8_t c = 0; c<len; c++) {
        uart_send_char(buf[c]);
    }
}

void uart_send_0(const char *text) {
    for(uint8_t c = 0; text[c]; c++) {
        uart_send_char((uint8_t)text[c]);
    }
}

void uart_send_char(uint8_t c) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}
