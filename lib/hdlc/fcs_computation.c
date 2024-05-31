// #include "fcs_computation.h"
// #include <stdint.h>
// #include <stdio.h>
// #include <assert.h>
// #include <stdbool.h>

// /*
//  * Calculate a new fcs given the current fcs and the new data.
//  */
// uint16_t calFcs16(uint16_t fcs, const uint8_t *cp, int len)
// {
//   assert(sizeof(uint16_t) == 2);
//   assert(len >= 0);

//   for (int i = 0; i < len; ++i)
//   {
//     fcs = (fcs >> 8) ^ fcstab[(fcs ^ cp[i]) & 0xff];
//     if (i == (len - 3))
//     {
//       uint16_t trialfcstmp = fcs;
//       unsigned char tmp;
//       tmp = trialfcstmp & 0xff;
//       printf("%02X ", tmp);
//       tmp = (trialfcstmp >> 8) & 0xff;
//       printf("%02X\n", tmp);
//     }
//   }
//   uint16_t trialfcstmp = fcs;
//   unsigned char tmp;
//   tmp = trialfcstmp & 0xff;
//   printf("%02X ", tmp);
//   tmp = (trialfcstmp >> 8) & 0xff;
//   printf("%02X\n", tmp);

//   return fcs;
// }

// bool testFcs16(unsigned char *cp, int len)
// {
//   bool check = false;
//   uint16_t trialfcs;

//   /* Add on output */
//   trialfcs = calFcs16(INITFCS16, cp, len);
//   trialfcs ^= 0xffff; // Complement

//   /* Write FCS to the end of the buffer */
//   cp[len] = trialfcs & 0xff;
//   cp[len + 1] = (trialfcs >> 8) & 0xff;

//   /* Check on input */
//   trialfcs = calFcs16(INITFCS16, cp, len + 2);
//   if (trialfcs == GOODFCS16)
//   {
//     printf("Good FCS\n");
//     check = true;
//   }
//   return check;
// }
