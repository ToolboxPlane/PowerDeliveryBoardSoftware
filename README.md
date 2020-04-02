# PowerDeliveryBoardSoftware
## Building and deploy
### Compile
To compile the firmware run:
```
cmake . && make
```
To compile the tests run the same commands in the test directory

### How to flash to firmware
This assumes that an AVRISP mk2 is used, if you use a different programmer
change the ```-c``` flag.
```bash
avrdude -p m2560 -B 10 -c avrisp2 -U flash:w:PowerDeliveryBoardSoftware.hex:i
```

### Fuses
The fuses select the "Ext. Crystal Osc.; Frequency 8.0-    MHz; Start-up time: 16K CK + 4.1 ms; [CKSEL=1111 SUT=10]"
clock, disables the clock divider, enables the preserve eeprom flag, enables the watchdog timer, enables SPI and disables
JTAG and OCD. The brownout detection level is set to 1.8V.

| Fusebyte | Value |
|--- | --- |
| Low | 0xEF |
| High | 0xC1 |
| Extended | 0xFE |

#### How to flash the fuses
```bash
avrdude -p m2560 -B 10 -c avrisp2 -U lfuse:w:0xef:m -U hfuse:w:0xc1:m -U efuse:w:0xfe:m
```

## LEDs
Under normal use all leds should be either on or blinking. By inverting most states one can be sure that all leds are working.

If only a single led is off for a longer time, there is a some kind of problem.

| LED-Number | Meaning |
| --- | --- |
| 0 | Alive (blinks regularly) |
| 1 | Not Watchdog |
| 2 | Not Brownout |
| 3 | I²C for VCC OK|
| 4 | I²C for 5V OK |
| 5 | VCC (Voltage and Current) OK |
| 6 | 5V (Voltage and Current) OK |
| 7 | Unused |

## Package format (Transmitter ID 74)
The package is a 8 bit, 4 Channel Package with the following data:

| Command | Response |
| --- | --- |
| 0 | Voltage VCC in mV / 128 |
| 1 | Current VCC in mA / 256 |
| 2 | Voltage 5V in mV / 32 |
| 3 | Current 5V in mA / 64 |
