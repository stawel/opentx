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

#include "common.h"
#include "avr_spi.h"
#include "protocol/interface.h"
#include "config/model.h"
#include "config/tx.h"

//volatile s32 Channels[NUM_OUT_CHANNELS];
struct Model Model;
struct Transmitter Transmitter;

void CLOCK_setPeriod(unsigned us);


static unsigned (*CLOCK_cb)(void);
static unsigned CLOCK_us;
uint32_t CLOCK_us_sum = 0;

void PROTOCOL_SetBindState(u32 msec)
{
    //TODO: ??
}

void CLOCK_StopTimer()
{
    //TODO: ??
}
void CLOCK_ResetWatchdog()
{
    //TODO: ??
}

uint32_t CLOCK_getms()
{
    while(TCNT1 > OCR1A) {
        CLOCK_us_sum += OCR1A/2;
        TCNT1 -= OCR1A;
    }
    uint32_t ms = CLOCK_us_sum;
    ms += TCNT1/1;
    ms /= 1000;
    return ms;
}

void CLOCK_StartTimer(unsigned us, unsigned (*cb)(void))
{
    CLOCK_cb=cb;
    CLOCK_setPeriod(us);
}
void CLOCK_setPeriod(unsigned us)
{
    CLOCK_us=us;
    OCR1A = CLOCK_us*2;
}

static void setup_pulses()
{
    static uint8_t nulls[2] = {0,0};
    OCR1A = CLOCK_us*2;
    TIMSK |= 0x10;                        // Enable COMPA
    TCCR1A = (0 << WGM10);
    TCCR1B = (1 << WGM12) | (2 << CS10);  // CTC OCRA, 16MHz / 8
    pulses2MHzRPtr = nulls;
}

static void setup_deviation()
{
    CLOCK_setPeriod(22000);
    setup_pulses();
    CLOCK_us_sum = 0;
    CLOCK_cb = NULL;

    Model.num_channels = 8;
    Model.tx_power = TXPOWER_150mW;
}

#define PROTODEF(proto, module, map, cmd, name) extern const void * cmd(enum ProtoCmds);
#include "protocol/protocol.h"
#undef PROTODEF


void setupDeviationTx(uint8_t required_protocol)
{
    PAUSE_10MS_INTERRUPT();
    SPI_MasterInit();
    setup_deviation();
    switch(required_protocol) {
#define PROTODEF(proto, module, map, cmd, name) case proto: cmd(PROTOCMD_BIND); break;
#include "protocol/protocol.h"
#undef PROTODEF
    }
    SPI_MasterRelese();
    RESUME_10MS_INTERRUPT();
}

void runDeviationTx(uint8_t required_protocol)
{
    setup_pulses();
    PAUSE_10MS_INTERRUPT();
    SPI_MasterInit();
    CLOCK_us_sum += CLOCK_us;

    if(CLOCK_cb != NULL) {
        CLOCK_setPeriod(CLOCK_cb());
    }
    SPI_MasterRelese();
    RESUME_10MS_INTERRUPT();
}


