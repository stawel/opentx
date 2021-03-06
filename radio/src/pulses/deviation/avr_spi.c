/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

/*The following will force the loading of various
  functions used in the protocol modules, but unused elsewhere
  in Deviation.
  Note that we lie aboiut the arguments to these functions. It is
  Important that the actual functions never execute
*/

#include <avr/cpufunc.h>

#define DDR_SPI         DDRB
#define PORT_SPI        PORTB
#define PIN_SPI         PINB

#define DD_SCK          1
#define DD_MOSI         2
#define DD_MISO         3


#define DDR_CS          DDRC
#define PORT_CS         PORTC
#define DD_CS           6       //PC6, pin 41

#define DDR_CE          DDRC
#define PORT_CE         PORTC
#define DD_CE           7       //PC6, pin 41


static uint8_t savedDDR_SPI;
static uint8_t savedPORT_SPI;

void SPI_MasterInit(void)
{
    //enable CS pin
    DDR_CS |= (1<<DD_CS);
    DDR_CE |= (1<<DD_CE);
    PORT_CE |= (1<<DD_CE);

    savedDDR_SPI = DDR_SPI;
    savedPORT_SPI = PORT_SPI;
    /* Set MOSI and SCK output, all others input */
    DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK);
    /* Enable SPI, Master, set clock rate fck/16, mode 0 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //| (1<<CPOL)|(1<<CPHA);
}

void SPI_MasterRelese(void)
{
    SPCR = 0;

    //restore keys high state
    PORT_SPI |= (1<<DD_MISO) | (1<<DD_MOSI) | (1<<DD_SCK);
    DDR_SPI  |= (1<<DD_MISO) | (1<<DD_MOSI) | (1<<DD_SCK);
    usleep(20);

    DDR_SPI = savedDDR_SPI;
    PORT_SPI = savedPORT_SPI;

    PORT_CE &= ~(1<<DD_CE);
}

void cs_high()
{
    PORT_CS |= (1<<DD_CS);
}

void cs_low()
{
    PORT_CS &= ~(1<<DD_CS);
}

uint8_t spi_xfer(uint8_t byte)
{
    SPDR = byte;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

void _SPI_DELAY()
{
    for(volatile int i = 0; i < 5; i++)
        _NOP();
}
uint8_t PROTOSPI_read3wire()
{
    uint8_t rx = 0, DDR_SPI_save, PORT_SPI_save, SPCR_save;
    int i=8;

    DDR_SPI_save = DDR_SPI;
    PORT_SPI_save = PORT_SPI;
    SPCR_save = SPCR;
    DDR_SPI &= ~(1<<DD_MOSI);
    PORT_SPI &= ~(1<<DD_MOSI);
    PORT_SPI &= ~(1<<DD_SCK);
    SPCR = 0;

    while (i--) {
        _SPI_DELAY();
        PORT_SPI |= (1<<DD_SCK);
        rx <<= 1;
        rx |= PIN_SPI & (1<< DD_MOSI) ? 1 : 0;
        _SPI_DELAY();
        PORT_SPI &=  ~(1<<DD_SCK);
    }
    _SPI_DELAY();

    SPCR = SPCR_save;
    DDR_SPI = DDR_SPI_save;
    PORT_SPI = PORT_SPI_save;

    return rx;
}


