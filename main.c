#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <util/twi.h>

#include "uart.h"


/*
 * LTC-I²C Adresses
 * V+ 0xDE
 * 5V 0xCE
 * 3V3 0xD2
 */

typedef struct {
    uint32_t curr_power;
    uint32_t max_power;
    uint32_t min_power;
    uint16_t curr_voltage;
    uint16_t max_voltage;
    uint16_t min_voltage;
    uint32_t energy;
} ltc_result_t;

uint32_t power = 0;


// SPI-Serial transfer complete
ISR(SPI_STC_vect) {
    uint8_t data = SPDR;

    SPDR = 17;
}

// Twi-Interrupt
ISR(TWI_vect) {
}


int main() {
    // Disable the interrupts
    cli();

    // Setup the Pins
    DDRB = 0b00010000;
    DDRC = 0b00000000;
    DDRD = 0b11000010;

    // Setup I²C (SCL-Frequency 100kHz)
    TWBR = 23; // Twi-Bit-Rate-Register
    TWSR = 0b01; // Prescaler to 4, Clear all status flags
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE); // Enable Ack | Twi Enable | Twi Interrupt Enable

    // Setup SPI-Slave
    SPCR = (1 << SPIE) | (1 << SPE); // Interrupt enabled, Spi enabled, Slave-Mode

    // Setup UART
    uart_init(9600);

    // Enable the interrupts
    sei();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        TWCR = (1<<TWINT) | (1<<TWSTA)|(1<<TWEN); // Send start condition
        while (!(TWCR & (1<<TWINT)));
        if ((TWSR & 0xF8) != TW_START) {
            uart_send_char('E');
        }

        TWDR = 0xDE; // V+
        TWCR = (1<<TWINT) | (1<<TWEN);
        while (!(TWCR & (1<<TWINT)));
        if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
            uart_send_char('D');
        }

        TWDR = 0x05; // Power
        TWCR = (1<<TWINT) | (1<<TWEN);
        while (!(TWCR & (1<<TWINT)));
        if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
            uart_send_char('F');
        }

        TWCR = (1<<TWINT)|
               (1<<TWEN)|(1<<TWSTO);


        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
        while (!(TWCR & (1<<TWINT)));
        if ((TWSR & 0xF8) != TW_START) {
            uart_send_char('K');
        }

        TWDR = 0xDE | 0b1;
        TWCR = (1<<TWINT) | (1<<TWEN);
        while (!(TWCR & (1<<TWINT)));
        if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
            uart_send_char('J');
        }

        power = (uint32_t)TWDR << 16;
        while (!(TWCR & (1<<TWINT)));

        power |= (uint32_t)TWDR << 8;
        while (!(TWCR & (1<<TWINT)));

        power |= (uint32_t)TWDR;
        while (!(TWCR & (1<<TWINT)));

        TWCR = (1<<TWINT)|
               (1<<TWEN)|(1<<TWSTO);

        uart_send_char(power);
        /*
         * PD6 -> High, PD7 -> Low
         */
       /* PORTD |= (1 << 6);
        PORTD &= ~(1 << 7);
        _delay_ms(1000);

        PORTD |= (1 << 6);
        PORTD |= (1 << 7);
        _delay_ms(333);

        PORTD &= ~(1 << 6);
        PORTD &= ~(1 << 7);
        _delay_ms(333);

        PORTD &= ~(1 << 6);
        PORTD |= (1 << 7);
        _delay_ms(333);*/
    }
#pragma clang diagnostic pop
}