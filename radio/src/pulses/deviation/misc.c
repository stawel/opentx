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

u32 Crc(const void *buffer, u32 size)
{
   u32 crc = ~0;
   const u8  *position = (const u8 *)buffer;

   while (size--) 
      {
      int bit;
      crc ^= *position++;
      for (bit = 0; bit < 8; bit++) 
         {
         s32 out = crc & 1;
         crc >>= 1;
         crc ^= -out & 0xEDB88320;
         }
      }
   return ~crc;
}

