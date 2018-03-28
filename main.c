#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "uart.h"
#include "i2c/i2cmaster.h"

#include "ltc2946.h"
#include "io.h"
#include "at30ts74.h"

#define _DEBUG

#define LTC_VCC_ADDR 0xDE
#define LTC_5V_ADDR 0xCE
#define LTC_3V3_ADDR 0xD2
#define TEMP_ADDR 0b10010000

#define F(x) (uint8_t*)x,sizeof(x)

enum {
    VCC, _5V, _3V3
};

int8_t temp;
ltc_result_t ltc_measurements[3];

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
#ifdef _DEBUG
    uart_init(9600);
#endif

    // Set the Alarms
    ltc2946_setup(LTC_VCC_ADDR, 0, 0xFFFF, 14000, 20000, 0, 1000);
    ltc2946_setup(LTC_5V_ADDR, 0, 0xFFFF, 4500, 5500, 0, 500);
    at30ts74_setup(TEMP_ADDR, -128, 80);

    // Enable the interrupts
    sei();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        // VCC
        ltc2946_read(LTC_VCC_ADDR, &ltc_measurements[VCC]);

        // 5V
        ltc2946_read(LTC_5V_ADDR, &ltc_measurements[_5V]);

        // Temperature
        temp = at30ts74_read(TEMP_ADDR);

#ifdef _DEBUG
        char buf[12];
        uart_send_buf(F("Temp [C]: "));
        itoa(temp, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | Temp Alert: "));
        uart_send_char(ALERT_TEMP + '0');

        uart_send_buf(F(" | VCC [mV]: "));
        utoa(ltc_measurements[VCC].voltageMilliVolts, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | VCC [mA]: "));
        utoa(ltc_measurements[VCC].currentMilliAmperes, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | VCC [mJ]: "));
        utoa(ltc_measurements[VCC].energyMilliJoules, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | VCC Alert: "));
        uart_send_char(ALERT_VCC + '0');

        uart_send_buf(F(" | 5V [mV]: "));
        utoa(ltc_measurements[_5V].voltageMilliVolts, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | 5V [mA]: "));
        utoa(ltc_measurements[_5V].currentMilliAmperes, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | 5V [mJ]: "));
        utoa(ltc_measurements[_5V].energyMilliJoules, buf, 10);
        uart_send_0(buf);

        uart_send_buf(F(" | 5V Alert: "));
        uart_send_char(ALERT_5V + '0');

        uart_send_buf(F("\r\n"));
#endif
    }
#pragma clang diagnostic pop
}