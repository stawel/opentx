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


#ifdef MODULAR
  //Allows the linker to properly relocate
  #define M9912_Cmds PROTO_Cmds
  #pragma long_calls
#endif
#include "common.h"
#include "interface.h"
#include "mixer.h"
#include "config/model.h"
#include "config/tx.h" // for Transmitter
#include "music.h"

#ifdef MODULAR
  //Some versions of gcc apply this to definitions, others to calls
  //So just use long_calls everywhere
  //#pragma long_calls_off
  extern unsigned _data_loadaddr;
  const unsigned long protocol_type = (unsigned long)&_data_loadaddr;
#endif

#ifdef PROTO_HAS_NRF24L01

#include "iface_nrf24l01.h"

#ifdef EMULATOR
#define USE_FIXED_MFGID
#define BIND_COUNT 20
#define dbgprintf printf
#else
#undef BIND_COUNT
#define BIND_COUNT 900  // 6 seconds
//printf inside an interrupt handler is really dangerous
//this shouldn't be enabled even in debug builds without explicitly
//turning it on
#define dbgprintf(...)
#endif

#define PACKET_PERIOD    2640 // 2640 = 2.64us

#define INITIAL_WAIT     500
#define PACKET_SIZE 9   // packets have 9-byte payload

static const char * const m9912_opts[] = {
    NULL
};

enum {
    LAST_PROTO_OPT,
};
ctassert(LAST_PROTO_OPT <= NUM_PROTO_OPTS, too_many_protocol_opts);

// For code readability
enum {
    CHANNEL1 = 0, // Aileron
    CHANNEL2,     // Elevator
    CHANNEL3,     // Throttle
    CHANNEL4,     // Rudder
    CHANNEL5,     // Trim L/R
    CHANNEL6,     // Trim F/B
    CHANNEL7,     // flip
    CHANNEL8,     // speed
};

enum{
    // flags going to packet[6]
    FLAG_SPEED    = 0x01,
    FLAG_FLIP     = 0x80,
};

enum {
    M9912_INIT1 = 0,
    M9912_BIND2,
    M9912_DATA
};

static u8 packet[PACKET_SIZE];
static u16 counter;
static u8 tx_power;

static const u8 tx_addr[]      = {0xb2, 0xbe, 0x00, 0xcc, 0xcc};
static const u8 bind_tx_addr[] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc};

static u8 phase;

// Bit vector from bit position
#define BV(bit) (1 << bit)

static u8 rf_channel_idx = 0;
static u8 rf_channel_shift = 0;
static const u8 rf_channel_list[] = {
         2, 72, 12, 62, 22, 52, 32, 42,
        42, 32, 52, 22, 62, 12, 72,  2
};

static u8 getChannel()
{
    u8 ch = rf_channel_list[rf_channel_idx++] + rf_channel_shift;
    rf_channel_idx &= 0xf;
    return ch;
}

static u8 getChannelId()
{
    return ((rf_channel_idx & 1) << 4) + (((rf_channel_idx >>1) + 1)&7);
}

static u8 scale_channel(u8 ch, s16 destMin, s16 destMax)
{
    s32 a = Channels[ch] - CHAN_MIN_VALUE;
    a *= (destMax - destMin);
    a /= CHAN_MAX_VALUE - CHAN_MIN_VALUE;
    return destMin + a;
}

static void send_packet(u8 bind)
{
    if(bind) {
        //some constants
        packet[0] = 0x20;
        packet[1] = 0x14;
        packet[2] = 0x03;
        packet[3] = 0x25;

        //tx address, TODO: should be dynamic
        packet[4] = 0xB2;
        packet[5] = 0xBE;
        packet[6] = 0x00;

        //hopping?? first digit is probably the hopping shift
        packet[7] = 0x70;

        packet[8] = 0xaa;
    } else {
        rf_channel_shift = 7;

        packet[0] = scale_channel(CHANNEL3, 0xe1, 0x00);
        packet[1] = scale_channel(CHANNEL1, 0xe1, 0x00);
        packet[2] = scale_channel(CHANNEL4, 0x00, 0xe1);
        packet[3] = scale_channel(CHANNEL2, 0x00, 0xe1);

        //trim
        packet[4] = scale_channel(CHANNEL5, 0x01, 0x3f);
        packet[5] = scale_channel(CHANNEL6, 0x01, 0x3f);

        //speed, flip
        packet[6] = 0x00;

        if(Channels[CHANNEL7] > 0)
            packet[6] |= FLAG_FLIP;
        if(Channels[CHANNEL8] > 0)
            packet[6] |= FLAG_SPEED;

        //?? hopping ??
        packet[7] = getChannelId();

        packet[8] = 0x70;
        for(int i = 0; i < 8; i++)
            packet[8] += packet[i];
    }

    NRF24L01_WriteReg(NRF24L01_05_RF_CH, getChannel());

    // clear packet status bits and TX FIFO
    NRF24L01_WriteReg(NRF24L01_07_STATUS, 0x70);
    NRF24L01_FlushTx();

    XN297_WritePayload(packet, PACKET_SIZE);

    // Check and adjust transmission power. We do this after
    // transmission to not bother with timeout after power
    // settings change -  we have plenty of time until next
    // packet.
    if (tx_power != Model.tx_power) {
        //Keep transmit power updated
        tx_power = Model.tx_power;
        NRF24L01_SetPower(tx_power);
    }
}

static void m9912_init()
{
    NRF24L01_Initialize();

    NRF24L01_SetTxRxMode(TX_EN);

    // SPI trace of stock TX has these writes to registers that don't appear in
    // nRF24L01 or Beken 2421 datasheets.  Uncomment if you have an XN297 chip?
    //M9912
    // NRF24L01_WriteRegisterMulti(0x3f, "\x4c\x84\x6f\x9c\x20", 5);
    // old                         0x3f, "\x4c\x84\x67\x9c\x20", 5);

    // NRF24L01_WriteRegisterMulti(0x3e, "\xc9\x9a\xb0\x61\xbb\xab\x9c", 7);
    // same as cg023

    // NRF24L01_WriteRegisterMulti(0x39, "\x0b\xdf\xc4\xa7\x03", 5);
    // old                         0x39, "\x0b\xdf\xc4\xa7\x03\xab\x9c", 7);

    XN297_SetTXAddr(bind_tx_addr, 5);

    NRF24L01_FlushTx();
    NRF24L01_FlushRx();
    NRF24L01_WriteReg(NRF24L01_07_STATUS, 0x70);     // Clear data ready, data sent, and retransmit
    NRF24L01_WriteReg(NRF24L01_01_EN_AA, 0x00);      // No Auto Acknowldgement on all data pipes
    NRF24L01_WriteReg(NRF24L01_02_EN_RXADDR, 0x01);  // Enable data pipe 0 only
    NRF24L01_SetBitrate(NRF24L01_BR_1M);             // 1Mbps
    NRF24L01_SetPower(Model.tx_power);

    NRF24L01_WriteReg(NRF24L01_06_RF_SETUP, 0x07);

    // this sequence necessary for module from stock tx
    NRF24L01_ReadReg(NRF24L01_1D_FEATURE);
    NRF24L01_Activate(0x73);                          // Activate feature register
    NRF24L01_ReadReg(NRF24L01_1D_FEATURE);

    NRF24L01_WriteReg(NRF24L01_1C_DYNPD, 0x00);       // Disable dynamic payload length on all pipes
    NRF24L01_WriteReg(NRF24L01_1D_FEATURE, 0x00);     // Set feature bits on

    // Check for Beken BK2421/BK2423 chip
    // It is done by using Beken specific activate code, 0x53
    // and checking that status register changed appropriately
    // There is no harm to run it on nRF24L01 because following
    // closing activate command changes state back even if it
    // does something on nRF24L01
    NRF24L01_Activate(0x53); // magic for BK2421 bank switch
    dbgprintf("Trying to switch banks\n");
    if (NRF24L01_ReadReg(NRF24L01_07_STATUS) & 0x80) {
        dbgprintf("BK2421 detected\n");
        // Beken registers don't have such nice names, so we just mention
        // them by their numbers
        // It's all magic, eavesdropped from real transfer and not even from the
        // data sheet - it has slightly different values
        NRF24L01_WriteRegisterMulti(0x00, (u8 *) "\x40\x4B\x01\xE2", 4);
        NRF24L01_WriteRegisterMulti(0x01, (u8 *) "\xC0\x4B\x00\x00", 4);
        NRF24L01_WriteRegisterMulti(0x02, (u8 *) "\xD0\xFC\x8C\x02", 4);
        NRF24L01_WriteRegisterMulti(0x03, (u8 *) "\x99\x00\x39\x21", 4);
        NRF24L01_WriteRegisterMulti(0x04, (u8 *) "\xD9\x96\x82\x1B", 4);
        NRF24L01_WriteRegisterMulti(0x05, (u8 *) "\x24\x06\x7F\xA6", 4);
        NRF24L01_WriteRegisterMulti(0x0C, (u8 *) "\x00\x12\x73\x00", 4);
        NRF24L01_WriteRegisterMulti(0x0D, (u8 *) "\x46\xB4\x80\x00", 4);
        NRF24L01_WriteRegisterMulti(0x04, (u8 *) "\xDF\x96\x82\x1B", 4);
        NRF24L01_WriteRegisterMulti(0x04, (u8 *) "\xD9\x96\x82\x1B", 4);
    } else {
        dbgprintf("nRF24L01 detected\n");
    }
    NRF24L01_Activate(0x53); // switch bank back

    XN297_Configure(BV(NRF24L01_00_EN_CRC) | BV(NRF24L01_00_CRCO) | BV(NRF24L01_00_PWR_UP));
}

MODULE_CALLTYPE
static u16 m9912_callback()
{
    switch (phase) {
    case M9912_INIT1:
        MUSIC_Play(MUSIC_TELEMALARM1);
        phase = M9912_BIND2;
        break;

    case M9912_BIND2:
        if (counter == 0) {
            phase = M9912_DATA;
            XN297_SetTXAddr(tx_addr, 5);  //set transition address
            PROTOCOL_SetBindState(0);
            MUSIC_Play(MUSIC_DONE_BINDING);
        } else {
            send_packet(1);
            counter -= 1;
        }
        break;

    case M9912_DATA:
        send_packet(0);
        break;
    }
    return PACKET_PERIOD;
}

static void initialize()
{
    CLOCK_StopTimer();
    tx_power = Model.tx_power;
    
    counter = BIND_COUNT;
    m9912_init();
    phase = M9912_INIT1;

    PROTOCOL_SetBindState(BIND_COUNT * ((long)PACKET_PERIOD) / 1000);
    CLOCK_StartTimer(INITIAL_WAIT, m9912_callback);
}

const void *m9912_Cmds(enum ProtoCmds cmd)
{
    switch(cmd) {
        case PROTOCMD_INIT:  initialize(); return 0;
        case PROTOCMD_DEINIT:
        case PROTOCMD_RESET:
            CLOCK_StopTimer();
            return (void *)(NRF24L01_Reset() ? 1L : -1L);
        case PROTOCMD_CHECK_AUTOBIND: return (void *)1L; // always Autobind
        case PROTOCMD_BIND:  initialize(); return 0;
        case PROTOCMD_NUMCHAN: return (void *) 10L; // A, E, T, R, light, flip, photo, video, headless, rate
        case PROTOCMD_DEFAULT_NUMCHAN: return (void *)10L;
        case PROTOCMD_CURRENT_ID: return Model.fixed_id ? (void *)((unsigned long)Model.fixed_id) : 0;
        case PROTOCMD_GETOPTIONS: return m9912_opts;
        case PROTOCMD_TELEMETRYSTATE: return (void *)(long)PROTO_TELEM_UNSUPPORTED;
        default: break;
    }
    return 0;
}
#endif
