#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "i2c/i2cmaster.h"

#define _DEBUG

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE
#define LTC_3V3_ADDR 0xD2
#define TEMP_ADDR 0b10010000

typedef struct {
    uint16_t voltageMilliVolts;
    uint16_t currentMilliAmperes;
    uint32_t energyMilliJoules;
} ltc_result_t;

enum {
    VCC, _5V, _3V3
};

int8_t temp;
ltc_result_t ltc_measurements[3];

/*
 * LTC-Werte
 * 12.5mA / LSB
 * 25mv / LSB
 * 312.5 µW / LSB
 * 3279.086μC/LSB
 * 335.78mJ/LSB
 */

void getLtcMeasurements(uint8_t deviceAddr, ltc_result_t *result) {
    uint32_t tmp;

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x1E);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp= (uint16_t)i2c_readAck() << 4;
    tmp |= i2c_readNak() >> 4;
    result->voltageMilliVolts = tmp*25;
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x14);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp= (uint16_t)i2c_readAck() << 4;
    tmp |= i2c_readNak() >> 4;
    result->currentMilliAmperes = (uint16_t)(tmp*12.5);
    i2c_stop();

    i2c_start(deviceAddr | I2C_WRITE);
    i2c_write(0x3C);
    i2c_rep_start(deviceAddr | I2C_READ);
    tmp = (uint32_t)i2c_readAck() << 24;
    tmp |= (uint32_t)i2c_readAck() << 16;
    tmp |= (uint32_t)i2c_readAck() << 8;
    tmp |= (uint32_t)i2c_readNak();
    result->energyMilliJoules = (uint16_t)(tmp*335.78);
    i2c_stop();
}

void setReset(bool reset) {
    if(reset) {
        PORTD |= (1 << 6); // Reset -> Low
        PORTD &= ~(1 << 7);
    } else {
        PORTD &= ~(1 << 6); // Reset -> High
        PORTD |= (1 << 7);
    }
}

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
        // VCC
        getLtcMeasurements(LTC_VCC_ADDR, &ltc_measurements[VCC]);

        // 5V
        getLtcMeasurements(LTC_5V_ADDR, &ltc_measurements[_5V]);

        // Temperature
        i2c_start(TEMP_ADDR | I2C_WRITE);
        i2c_write(0x00);
        i2c_rep_start(TEMP_ADDR | I2C_READ);
        temp = i2c_readNak();
        i2c_stop();

#ifdef _DEBUG
        uart_send_char('T');
        uart_send_char(temp);
        uart_send_char('V');
        uart_send_char(ltc_measurements[VCC].voltageMilliVolts / 1000);
        uart_send_char('A');
        uart_send_char(ltc_measurements[VCC].currentMilliAmperes);
        uart_send_char('E');
        uart_send_char(ltc_measurements[VCC].energyMilliJoules / 1000);
        uart_send_char('5');
        uart_send_char(ltc_measurements[_5V].voltageMilliVolts / 1000);
        uart_send_char('A');
        uart_send_char(ltc_measurements[_5V].currentMilliAmperes);
        uart_send_char('E');
        uart_send_char(ltc_measurements[_5V].energyMilliJoules / 1000);
#endif
    }
#pragma clang diagnostic pop
}