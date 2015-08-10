#ifndef _AVR_PROTO_H_
#define _AVR_PROTO_H_

#include <deviationTx_glue.h>

#define PROTOSPI_pin_set(io)            cs_high()
#define PROTOSPI_pin_clear(io)          cs_low()
#define PROTOSPI_xfer(byte)             spi_xfer(byte)

void cs_high();
void cs_low();
uint8_t spi_xfer(uint8_t byte);

void SPI_MasterInit(void);
void SPI_MasterRelese(void);
uint8_t PROTOSPI_read3wire();

#endif
