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