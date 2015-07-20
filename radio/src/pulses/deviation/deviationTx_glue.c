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

#define DEVIATION_TX_PROTOCOL_CMD_CONCAT(x,y) x##y
#define DEVIATION_TX_PROTOCOL_CMD_CONCAT2(x,y) DEVIATION_TX_PROTOCOL_CMD_CONCAT(x,y)
#define DEVIATION_TX_PROTOCOL_CMD DEVIATION_TX_PROTOCOL_CMD_CONCAT2(DEVIATION_TX_PROTOCOL, _Cmds)

//volatile s32 Channels[NUM_OUT_CHANNELS];
struct Model Model;
struct Transmitter Transmitter;

const void *DEVIATION_TX_PROTOCOL_CMD(enum ProtoCmds cmd);
void CLOCK_setPeriod(unsigned us);


static unsigned (*CLOCK_cb)(void);
static unsigned CLOCK_us;

void PROTOCOL_SetBindState(u32 msec)
{
    //TODO: ??
}

void CLOCK_StopTimer()
{
    //TODO: ??
}
void CLOCK_StartTimer(unsigned us, unsigned (*cb)(void))
{
    CLOCK_us=us;
    CLOCK_cb=cb;
    CLOCK_setPeriod(us);
}
void CLOCK_setPeriod(unsigned us)
{
    OCR1A = us*2;
}

static void setup_pulses()
{
//    OCR1A = 44000;                        // Next frame starts in 22ms
    CLOCK_setPeriod(22000);
    TIMSK |= 0x10;                        // Enable COMPA
    TCCR1A = (0 << WGM10);
    TCCR1B = (1 << WGM12) | (2 << CS10);  // CTC OCRA, 16MHz / 8
}


static void setup_deviation()
{
    Model.num_channels = 8;
    Model.tx_power = TXPOWER_150mW;
}

static void convert_channels()
{
     //if needed
/*    for (uint8_t i = 0; i < 8; i++) {
        Channels[i] = channelOutputs[i];
        Channels[i] *= CHAN_MAX_VALUE;
        Channels[i] /= 1024;
    }
    */
}

void setupDeviationTx()
{
    setup_pulses();
    setup_deviation();
    convert_channels();

    PAUSE_10MS_INTERRUPT();
    SPI_MasterInit();

    if(CLOCK_cb == NULL) {
#define XX ala
        DEVIATION_TX_PROTOCOL_CMD(PROTOCMD_INIT);
    }   else {
        CLOCK_setPeriod(CLOCK_cb());
    }
    SPI_MasterRelese();
    RESUME_10MS_INTERRUPT();
}


