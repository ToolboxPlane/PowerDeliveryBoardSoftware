//
// Created by paul on 27.03.18.
//

#include <avr/io.h>

#ifndef POWERDISTRIBUTIONBOARDSOFTWARE_UART_H
#define POWERDISTRIBUTIONBOARDSOFTWARE_UART_H

void uart_init(uint32_t baud);
void uart_send_buf(uint8_t *buf, uint8_t len);
void uart_send_0(const char *text);
void uart_send_char(uint8_t c);

#endif //POWERDISTRIBUTIONBOARDSOFTWARE_UART_H
