# PowerDistributionBoardSoftware
## Build
```
cmake .
make
```

## Flash
```
avrdude -p m48 -B 10 -c avrisp2 -U flash:w:PowerDistributionBoardSoftware.hex:i
```

## Fuses
```
FF D7 FF
```
### I²C Library from Peter Fleury, see: http://homepage.hispeed.ch/peterfleury/avr-software.html

### SPI-Api
The Master sends a single command byte to the slave which sends back the response during the next clock cycle.
Possible command are:

| Command | Response |
| --- | --- |
| 0 | Status |
| 1 | Voltage VCC in mV / 128 |
| 2 | Current VCC in mA / 256 |
| 3 | Voltage 5V in mV / 32 |
| 4 | Current 5V in mA / 64 |
| 5 | Temperature in °C |
