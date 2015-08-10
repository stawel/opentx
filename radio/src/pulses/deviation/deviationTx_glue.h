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

#ifndef DEVIATION_TX_GLUE_H_
#define DEVIATION_TX_GLUE_H_

#include "opentx.h"
#define printf_not_allowed(...)

void setupDeviationTx(uint8_t required_protocol);
void runDeviationTx(uint8_t required_protocol);

#define NUM_OUT_CHANNELS            8
//extern volatile int32_t             Channels[NUM_OUT_CHANNELS];
#define Channels                    channelOutputs
#define CHAN_MAX_VALUE              1024
#define CHAN_MIN_VALUE              -1024


#define MODULE_CALLTYPE
#define MUSIC_Play(x)
#define usleep(x)                   _delay_us(x)
#define PROTO_TELEM_UNSUPPORTED     -1


void CLOCK_StopTimer();
void CLOCK_StartTimer(unsigned us, unsigned (*cb)(void));
uint32_t CLOCK_getms();
void CLOCK_ResetWatchdog();

#endif
