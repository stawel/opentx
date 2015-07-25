##OpenTX m9912 Branch

Original repo: https://github.com/opentx/opentx

This is a "proof of concept" branch for the m9912 copter protocol,
it combines the OpenTx firmware with the DeviationTx protocol part,
in theory you should be able use any other DeviationTx protocol.

Tested on Tunring 9x,
uses nrf24l01 through atmega64 SPI.


nrf24l01 to atmega64 connection:
```
VCC    ->  3.3V (LCD power supply)
GND    ->  GND
MISO   ->  MISO (atmega64 pin 13)
MOSI   ->  MOSI (atmega64 pin 12)
CLK    ->  SCK  (atmega64 pin 11)
CSN    ->  PC6  (atmega64 pin 41, see: avr_spi.c)
CE     ->  PC7  (atmega64 pin 42, see: avr_spi.c, not really used)
IRQ    ->  not connected
```

channels:
```
Aileron  -> channel1
Elevator -> channel2
Throttle -> channel3
Rudder   -> channel4
Trim L/R -> channel5
Trim F/B -> channel6
flip     -> channel7
speed    -> channel8
```