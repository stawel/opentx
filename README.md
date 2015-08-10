##OpenTX m9912 Branch

Original repo: https://github.com/opentx/opentx

This is a "proof of concept" branch
it combines OpenTx firmware with the DeviationTx protocol part,
in theory you should be able use any DeviationTx protocol.

Tested on Tunring 9x,
uses nrf24l01 or a7105 through atmega64 SPI.


Tested Protocols: H7 through nrf24l01 module, flysky through a7105 module


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

a7105 to atmega64 connection:
```
VCC    ->  3.3V (LCD power supply)
GND    ->  GND
sdio   ->  MOSI (atmega64 pin 12)
sck    ->  SCK  (atmega64 pin 11)
scs    ->  PC6  (atmega64 pin 41, see: avr_spi.c)
```
