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

#ifndef DEVIATION_TX_STRINGS_H_
#define DEVIATION_TX_STRINGS_H_


//TODO: ??
//#define PROTODEF(proto, module, map, cmd, name) TR_DEVIATION_APPEND_STRING(__COUNTER__, name "\0")
//#include "protocol/protocol.h"

#ifdef PROTO_HAS_CYRF6936
#ifdef DEVTX_PROTOCOL_DEVO
#define PROTOCOL_DEVO_STRING        "DEVO\0 "
#endif

#ifdef DEVTX_PROTOCOL_WK2801
#define PROTOCOL_WK2801_STRING      "WK2801"
#endif
#ifndef DEFINE_FUNCS
#ifdef DEVTX_PROTOCOL_WK2601
#define  PROTOCOL_WK2601_STRING     "WK2601"
#endif
#ifdef DEVTX_PROTOCOL_WK2401
#define  PROTOCOL_WK2401_STRING     "WK2401"
#endif
#ifdef DEVTX_PROTOCOL_DSM2
#define  PROTOCOL_DSM2_STRING       "DSM2\0 "
#endif
#endif
#ifdef DEVTX_PROTOCOL_DSMX
#define  PROTOCOL_DSMX_STRING       "DSMX\0 "
#endif
#ifdef DEVTX_PROTOCOL_J6PRO
#define  PROTOCOL_J6PRO_STRING      "J6Pro\0"
#endif
#endif //PROTO_HAS_CYRF6936

#ifdef PROTO_HAS_A7105
#ifdef DEVTX_PROTOCOL_FLYSKY
#define  PROTOCOL_FLYSKY_STRING     "Flysky"
#endif
#ifdef DEVTX_PROTOCOL_HUBSAN
#define  PROTOCOL_HUBSAN_STRING     "Hubsn4"
#endif
#ifdef DEVTX_PROTOCOL_JOYSWAY
#define  PROTOCOL_JOYSWAY_STRING    "Joyswy"
#endif
#endif //PROTO_HAS_A7105

#ifdef PROTO_HAS_CC2500
#ifdef DEVTX_PROTOCOL_FRSKY2WAY
#define  PROTOCOL_FRSKY2WAY_STRING  "Frsky\0"
#endif
#ifdef DEVTX_PROTOCOL_FRSKY1WAY
#define  PROTOCOL_FRSKY1WAY_STRING  "Frs-V8"
#endif
#ifdef DEVTX_PROTOCOL_SKYARTEC
#define  PROTOCOL_SKYARTEC_STRING   "Skyrtc"
#endif
#endif //PROTO_HAS_CC2500

#ifdef PROTO_HAS_NRF24L01
#ifdef DEVTX_PROTOCOL_V202
#define  PROTOCOL_V202_STRING       "V202\0 "
#endif
#ifdef DEVTX_PROTOCOL_SLT
#define  PROTOCOL_SLT_STRING        "SLT\0  "
#endif
#ifdef DEVTX_PROTOCOL_HiSky
#define  PROTOCOL_HiSky_STRING      "HiSky\0"
#endif
#ifdef DEVTX_PROTOCOL_YD717
#define  PROTOCOL_YD717_STRING      "YD717\0"
#endif
#ifdef DEVTX_PROTOCOL_SymaX
#define  PROTOCOL_SymaX_STRING      "SymaX\0"
#endif
#ifdef DEVTX_PROTOCOL_CFlie
#define  PROTOCOL_CFlie_STRING      "CFlie\0"
#endif
#ifdef DEVTX_PROTOCOL_H377
#define  PROTOCOL_H377_STRING       "H377\0 "
#endif
#ifdef DEVTX_PROTOCOL_HM830
#define  PROTOCOL_HM830_STRING      "HM830\0"
#endif
#ifdef DEVTX_PROTOCOL_KN
#define  PROTOCOL_KN_STRING         "KN\0   "
#endif
#ifdef DEVTX_PROTOCOL_ESKY150
#define  PROTOCOL_ESKY150_STRING    "ESky150"
#endif
#ifdef DEVTX_PROTOCOL_ESKY
#define  PROTOCOL_ESKY_STRING       "ESky\0 "
#endif
#ifdef DEVTX_PROTOCOL_BLUEFLY
#define  PROTOCOL_BLUEFLY_STRING    "BlueFly"
#endif
#ifdef DEVTX_PROTOCOL_NE260
#define  PROTOCOL_NE260_STRING      "NE260\0"
#endif
#ifdef DEVTX_PROTOCOL_CX10
#define  PROTOCOL_CX10_STRING       "CX10\0 "
#endif
#ifdef DEVTX_PROTOCOL_CG023
#define  PROTOCOL_CG023_STRING      "CG023\0"
#endif
#ifdef DEVTX_PROTOCOL_M9912
#define  PROTOCOL_M9912_STRING      "m9912\0"
#endif
#ifdef DEVTX_PROTOCOL_H7
#define  PROTOCOL_H7_STRING         "H7\0   "
#endif
#endif //PROTO_HAS_NRF24L01



#ifndef PROTOCOL_DEVO_STRING
#define PROTOCOL_DEVO_STRING
#endif
#ifndef PROTOCOL_WK2801_STRING
#define PROTOCOL_WK2801_STRING
#endif
#ifndef PROTOCOL_WK2601_STRING
#define PROTOCOL_WK2601_STRING
#endif
#ifndef PROTOCOL_WK2401_STRING
#define PROTOCOL_WK2401_STRING
#endif
#ifndef PROTOCOL_DSM2_STRING
#define PROTOCOL_DSM2_STRING
#endif
#ifndef PROTOCOL_DSMX_STRING
#define PROTOCOL_DSMX_STRING
#endif
#ifndef PROTOCOL_J6PRO_STRING
#define PROTOCOL_J6PRO_STRING
#endif
#ifndef PROTOCOL_FLYSKY_STRING
#define PROTOCOL_FLYSKY_STRING
#endif
#ifndef PROTOCOL_HUBSAN_STRING
#define PROTOCOL_HUBSAN_STRING
#endif
#ifndef PROTOCOL_JOYSWAY_STRING
#define PROTOCOL_JOYSWAY_STRING
#endif
#ifndef PROTOCOL_FRSKY2WAY_STRING
#define PROTOCOL_FRSKY2WAY_STRING
#endif
#ifndef PROTOCOL_FRSKY1WAY_STRING
#define PROTOCOL_FRSKY1WAY_STRING
#endif
#ifndef PROTOCOL_SKYARTEC_STRING
#define PROTOCOL_SKYARTEC_STRING
#endif
#ifndef PROTOCOL_V202_STRING
#define PROTOCOL_V202_STRING
#endif
#ifndef PROTOCOL_SLT_STRING
#define PROTOCOL_SLT_STRING
#endif
#ifndef PROTOCOL_HiSky_STRING
#define PROTOCOL_HiSky_STRING
#endif
#ifndef PROTOCOL_YD717_STRING
#define PROTOCOL_YD717_STRING
#endif
#ifndef PROTOCOL_SymaX_STRING
#define PROTOCOL_SymaX_STRING
#endif
#ifndef PROTOCOL_CFlie_STRING
#define PROTOCOL_CFlie_STRING
#endif
#ifndef PROTOCOL_H377_STRING
#define PROTOCOL_H377_STRING
#endif
#ifndef PROTOCOL_HM830_STRING
#define PROTOCOL_HM830_STRING
#endif
#ifndef PROTOCOL_KN_STRING
#define PROTOCOL_KN_STRING
#endif
#ifndef PROTOCOL_ESKY150_STRING
#define PROTOCOL_ESKY150_STRING
#endif
#ifndef PROTOCOL_ESKY_STRING
#define PROTOCOL_ESKY_STRING
#endif
#ifndef PROTOCOL_BLUEFLY_STRING
#define PROTOCOL_BLUEFLY_STRING
#endif
#ifndef PROTOCOL_NE260_STRING
#define PROTOCOL_NE260_STRING
#endif
#ifndef PROTOCOL_CX10_STRING
#define PROTOCOL_CX10_STRING
#endif
#ifndef PROTOCOL_CG023_STRING
#define PROTOCOL_CG023_STRING
#endif
#ifndef PROTOCOL_M9912_STRING
#define PROTOCOL_M9912_STRING
#endif
#ifndef PROTOCOL_H7_STRING
#define PROTOCOL_H7_STRING
#endif


#define TR_DEVIATION_TX PROTOCOL_DEVO_STRING PROTOCOL_WK2801_STRING PROTOCOL_WK2601_STRING PROTOCOL_WK2401_STRING \
                        PROTOCOL_DSM2_STRING PROTOCOL_DSMX_STRING PROTOCOL_J6PRO_STRING PROTOCOL_FLYSKY_STRING \
                        PROTOCOL_HUBSAN_STRING PROTOCOL_JOYSWAY_STRING PROTOCOL_FRSKY2WAY_STRING \
                        PROTOCOL_FRSKY1WAY_STRING PROTOCOL_SKYARTEC_STRING PROTOCOL_V202_STRING \
                        PROTOCOL_SLT_STRING PROTOCOL_HiSky_STRING PROTOCOL_YD717_STRING PROTOCOL_SymaX_STRING \
                        PROTOCOL_CFlie_STRING PROTOCOL_H377_STRING PROTOCOL_HM830_STRING PROTOCOL_KN_STRING \
                        PROTOCOL_ESKY150_STRING PROTOCOL_ESKY_STRING PROTOCOL_BLUEFLY_STRING PROTOCOL_NE260_STRING \
                        PROTOCOL_CX10_STRING PROTOCOL_CG023_STRING PROTOCOL_M9912_STRING PROTOCOL_H7_STRING

#endif
