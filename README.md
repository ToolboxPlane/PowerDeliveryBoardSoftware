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