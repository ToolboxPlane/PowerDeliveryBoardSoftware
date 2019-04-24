# PowerDistributionBoardSoftware


# FlightController
## Building and deploy
### Compile
To compile the firmware run:
```
cmake . && make
```
To compile the tests run the same commands in the test directory
#### WTF? Two independent cmake-projects?
You may ask why is there a second, independet cmake project for the tests (and you have the best right to do so).
Well i do not like the solution either but it seems like cmake is not able to have two targets with different compilers.
So if you find a better solution for doing this (no i won't use Makefiles or Bazel or Scons or Gradle or whatever...) please write an issue and i will fix this immediatly.

### How to flash to firmware
This assumes that an AVRISP mk2 is used, if you use a different programmer
change the ```-c``` flag.
```bash
avrdude -p m2560 -B 10 -c avrisp2 -U flash:w:FlightController.hex:i
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

## Package format (Transmitter ID 74)
The package is a 8 bit, 8 Channel Package with the following data:

| Command | Response |
| --- | --- |
| 0 | Status |
| 1 | Voltage VCC in mV / 128 |
| 2 | Current VCC in mA / 256 |
| 3 | Voltage 5V in mV / 32 |
| 4 | Current 5V in mA / 64 |
| 5 | 0 |
