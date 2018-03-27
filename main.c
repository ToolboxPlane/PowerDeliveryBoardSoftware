#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <util/twi.h>

#include "uart.h"
#include "i2c/i2cmaster.h"

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE
#define LTC_3V3_ADDR 0xD2
#define TEMP_ADDR 0b10010000

typedef struct {
    uint32_t curr_power;
    uint32_t max_power;
    uint32_t min_power;
    uint16_t curr_voltage;
    uint16_t max_voltage;
    uint16_t min_voltage;
    uint32_t energy;
} ltc_result_t;

int8_t temp;

/*
 * LTC-Werte
 * 12.5mA / LSB
 * 25mv / LSB
 * 312.5 µW / LSB
 * 3279.086μC/LSB
 * 335.78mJ/LSB
 */


// SPI-Serial transfer complete
ISR(SPI_STC_vect) {
    uint8_t data = SPDR;

    SPDR = 17;
}

int main() {
    // Disable the interrupts
    cli();

    // Setup the Pins
    DDRB = 0b00010000;
    DDRC = 0b00000000;
    DDRD = 0b11000010;

    // Setup I²C (SCL-Frequency 100kHz)
    i2c_init();

    // Setup SPI-Slave
    SPCR = (1 << SPIE) | (1 << SPE); // Interrupt enabled, Spi enabled, Slave-Mode

    // Setup UART
    uart_init(9600);

    // Enable the interrupts
    sei();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        if(i2c_start(LTC_VCC_ADDR | I2C_WRITE)) {
            uart_send_char('a');
        }

        if(i2c_write(0x1E)) {
            uart_send_char('A');
        }

        if(i2c_rep_start(LTC_VCC_ADDR + I2C_READ)) {
            uart_send_char('B');
        }

        uart_send_char('C');
        uint16_t voltage = (uint16_t)i2c_readAck() << 4;
        voltage |= i2c_readNak() >> 4;
        uart_send_char(voltage/40);

        i2c_stop();

        i2c_start(TEMP_ADDR | I2C_WRITE);
        i2c_write(0x00);
        i2c_rep_start(TEMP_ADDR | I2C_READ);
        temp = i2c_readNak();
        i2c_stop();

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