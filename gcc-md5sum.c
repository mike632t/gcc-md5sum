/*
 * gcc-md5sum.c
 *
 * Copyright(C) 2025   MT
 *
 * A standalone implementation in ANSI C derived from the RSA Data Security
 * Inc. MD5 Message-Digest Algorithm in RFC 1321.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 25 Jul 25  0.1.0001  - Initial version - MT
 * 28 Jul 25            - Defining  DEBUG allows the routines to be  tested 
 *                        independently of the file system. - MT
 *                      - Added copyright notice from RFC - MT
 * 
 * TODO:                
 * 
 */

#define  NAME           "gcc-md5sum"
#define  VERSION        "0.1"
#define  BUILD          "0001"
#define  DATE           "25 Jul 25"
#define  AUTHOR         "MT"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>      /* errno */

#include "gcc-md5sum.h"
#include "gcc-debug.h"

int main(int argc, char *argv[])
{
   struct md5context t_context;
   unsigned char i_digest[16];
   unsigned char c_buffer[512];
   int i_counter;

#if defined(DEBUG)

   char *s_data = "abc";

   memcpy (c_buffer, s_data, strlen(s_data)); /* Copy test data to buffer */

   v_init(&t_context);
   v_update(&t_context, c_buffer, strlen(s_data));
   v_final(&t_context, i_digest);
   
   for (i_counter = 0; i_counter < 16; ++i_counter)
      printf("%02x", i_digest[i_counter]);
   printf("\n");
   printf("900150983cd24fb0d6963f7d28e17f72\n");

#else

   FILE *h_file;
   size_t i_bytes;
   int i_count;
   
   for (i_count = 1; i_count < argc; i_count++)
   {
      if ((h_file = fopen(argv[i_count], "rb"))) /* No difference between 'rb' and 'r' on linux */
      {
         v_init(&t_context);
         while ((i_bytes = fread(c_buffer, 1, sizeof(c_buffer), h_file)) > 0)
            v_update(&t_context, c_buffer, (unsigned int)i_bytes);
         v_final(&t_context, i_digest);
         
         for (i_counter = 0; i_counter < 16; ++i_counter)
            printf("%02x", i_digest[i_counter]);
         printf("  %s\n",argv[i_count]);
         fclose(h_file);
      }
   else
      warning(errno, "Could not open '%s'", argv[i_count]);
   }

#endif
   return errno;
}

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All 
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
   Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software. 
*/

void v_init(struct md5context *t_context) /* Initialize state (constants defined in RFC 1321) */
{
   t_context->count[0] = t_context->count[1] = 0;
   t_context->state[0] = 0x67452301;
   t_context->state[1] = 0xefcdab89;
   t_context->state[2] = 0x98badcfe;
   t_context->state[3] = 0x10325476;
}

void v_update(struct md5context *t_context, const unsigned char *c_buffer, unsigned int i_length)
{
   unsigned int i_count, i_size, i_offset;

   /* Compute number of uint8_ts mod 64 */
   i_offset = (unsigned int)((t_context->count[0] >> 3) & 0x3F);

   /* Update number of bits */
   if ((t_context->count[0] += ((unsigned int)i_length << 3)) < ((unsigned int)i_length << 3))
      t_context->count[1]++;
   t_context->count[1] += ((unsigned int)i_length >> 29);

   i_size = 64 - i_offset;

   /* Transform as many times as possible */
   if (i_length >= i_size) 
   {
      memcpy(&t_context->buffer[i_offset], c_buffer, i_size);
      v_transform(t_context->state, t_context->buffer);
      for (i_count = i_size; i_count + 63 < i_length; i_count += 64)
         v_transform(t_context->state, &c_buffer[i_count]);
      i_offset = 0;
   }
   else 
   {
      i_count = 0;
   }

   /* Buffer remaining input */
   memcpy(&t_context->buffer[i_offset], &c_buffer[i_count], i_length - i_count);
}

void v_final(struct md5context *t_context, unsigned char c_digest[16])
{
   static const unsigned char c_padding[64] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   unsigned char bits[8];
   unsigned int  i_size, i_offset;

   v_encode(t_context->count, bits, 8); /* Save number of bits */

   /* Pad out to 56 mod 64 */
   i_offset = (unsigned int)((t_context->count[0] >> 3) & 0x3f);
   i_size = (i_offset < 56) ? (56 - i_offset) : (120 - i_offset);
   v_update(t_context, c_padding, i_size);

   v_update(t_context, bits, 8); /* Append length (before padding) */
   v_encode(t_context->state, c_digest, 16); /* Store state in digest */
   
   /** memset (t_context, 0x0, sizeof (*t_context)); */
}

static void v_transform(unsigned int i_state[4], const unsigned char c_block[64]) /* Internal MD5 transformation */
{
   unsigned int a, b, c, d;
   unsigned int i_digest[16];

   a = i_state[0]; b = i_state[1]; c = i_state[2]; d = i_state[3];
   
   v_decode(c_block, i_digest, 64);

   /* Round 1 */
   FF(a, b, c, d, i_digest[ 0],  7, 0xd76aa478); /* 1 */
   FF(d, a, b, c, i_digest[ 1], 12, 0xe8c7b756); /* 2 */
   FF(c, d, a, b, i_digest[ 2], 17, 0x242070db); /* 3 */
   FF(b, c, d, a, i_digest[ 3], 22, 0xc1bdceee); /* 4 */
   FF(a, b, c, d, i_digest[ 4],  7, 0xf57c0faf); /* 5 */
   FF(d, a, b, c, i_digest[ 5], 12, 0x4787c62a); /* 6 */
   FF(c, d, a, b, i_digest[ 6], 17, 0xa8304613); /* 7 */
   FF(b, c, d, a, i_digest[ 7], 22, 0xfd469501); /* 8 */
   FF(a, b, c, d, i_digest[ 8],  7, 0x698098d8); /* 9 */
   FF(d, a, b, c, i_digest[ 9], 12, 0x8b44f7af); /* 10 */
   FF(c, d, a, b, i_digest[10], 17, 0xffff5bb1); /* 11 */
   FF(b, c, d, a, i_digest[11], 22, 0x895cd7be); /* 12 */
   FF(a, b, c, d, i_digest[12],  7, 0x6b901122); /* 13 */
   FF(d, a, b, c, i_digest[13], 12, 0xfd987193); /* 14 */
   FF(c, d, a, b, i_digest[14], 17, 0xa679438e); /* 15 */
   FF(b, c, d, a, i_digest[15], 22, 0x49b40821); /* 16 */

   /* Round 2 */
   GG(a, b, c, d, i_digest[ 1],  5, 0xf61e2562); /* 17 */
   GG(d, a, b, c, i_digest[ 6],  9, 0xc040b340); /* 18 */
   GG(c, d, a, b, i_digest[11], 14, 0x265e5a51); /* 19 */
   GG(b, c, d, a, i_digest[ 0], 20, 0xe9b6c7aa); /* 20 */
   GG(a, b, c, d, i_digest[ 5],  5, 0xd62f105d); /* 21 */
   GG(d, a, b, c, i_digest[10],  9, 0x02441453); /* 22 */
   GG(c, d, a, b, i_digest[15], 14, 0xd8a1e681); /* 23 */
   GG(b, c, d, a, i_digest[ 4], 20, 0xe7d3fbc8); /* 24 */
   GG(a, b, c, d, i_digest[ 9],  5, 0x21e1cde6); /* 25 */
   GG(d, a, b, c, i_digest[14],  9, 0xc33707d6); /* 26 */
   GG(c, d, a, b, i_digest[ 3], 14, 0xf4d50d87); /* 27 */
   GG(b, c, d, a, i_digest[ 8], 20, 0x455a14ed); /* 28 */
   GG(a, b, c, d, i_digest[13],  5, 0xa9e3e905); /* 29 */
   GG(d, a, b, c, i_digest[ 2],  9, 0xfcefa3f8); /* 30 */
   GG(c, d, a, b, i_digest[ 7], 14, 0x676f02d9); /* 31 */
   GG(b, c, d, a, i_digest[12], 20, 0x8d2a4c8a); /* 32 */

   /* Round 3 */
   HH(a, b, c, d, i_digest[ 5],  4, 0xfffa3942); /* 33 */
   HH(d, a, b, c, i_digest[ 8], 11, 0x8771f681); /* 34 */
   HH(c, d, a, b, i_digest[11], 16, 0x6d9d6122); /* 35 */
   HH(b, c, d, a, i_digest[14], 23, 0xfde5380c); /* 36 */
   HH(a, b, c, d, i_digest[ 1],  4, 0xa4beea44); /* 37 */
   HH(d, a, b, c, i_digest[ 4], 11, 0x4bdecfa9); /* 38 */
   HH(c, d, a, b, i_digest[ 7], 16, 0xf6bb4b60); /* 39 */
   HH(b, c, d, a, i_digest[10], 23, 0xbebfbc70); /* 40 */
   HH(a, b, c, d, i_digest[13],  4, 0x289b7ec6); /* 41 */
   HH(d, a, b, c, i_digest[ 0], 11, 0xeaa127fa); /* 42 */
   HH(c, d, a, b, i_digest[ 3], 16, 0xd4ef3085); /* 43 */
   HH(b, c, d, a, i_digest[ 6], 23, 0x04881d05); /* 44 */
   HH(a, b, c, d, i_digest[ 9],  4, 0xd9d4d039); /* 45 */
   HH(d, a, b, c, i_digest[12], 11, 0xe6db99e5); /* 46 */
   HH(c, d, a, b, i_digest[15], 16, 0x1fa27cf8); /* 47 */
   HH(b, c, d, a, i_digest[ 2], 23, 0xc4ac5665); /* 48 */

   /* Round 4 */
   II(a, b, c, d, i_digest[ 0],  6, 0xf4292244); /* 49 */
   II(d, a, b, c, i_digest[ 7], 10, 0x432aff97); /* 50 */
   II(c, d, a, b, i_digest[14], 15, 0xab9423a7); /* 51 */
   II(b, c, d, a, i_digest[ 5], 21, 0xfc93a039); /* 52 */
   II(a, b, c, d, i_digest[12],  6, 0x655b59c3); /* 53 */
   II(d, a, b, c, i_digest[ 3], 10, 0x8f0ccc92); /* 54 */
   II(c, d, a, b, i_digest[10], 15, 0xffeff47d); /* 55 */
   II(b, c, d, a, i_digest[ 1], 21, 0x85845dd1); /* 56 */
   II(a, b, c, d, i_digest[ 8],  6, 0x6fa87e4f); /* 57 */
   II(d, a, b, c, i_digest[15], 10, 0xfe2ce6e0); /* 58 */
   II(c, d, a, b, i_digest[ 6], 15, 0xa3014314); /* 59 */
   II(b, c, d, a, i_digest[13], 21, 0x4e0811a1); /* 60 */
   II(a, b, c, d, i_digest[ 4],  6, 0xf7537e82); /* 61 */
   II(d, a, b, c, i_digest[11], 10, 0xbd3af235); /* 62 */
   II(c, d, a, b, i_digest[ 2], 15, 0x2ad7d2bb); /* 63 */
   II(b, c, d, a, i_digest[ 9], 21, 0xeb86d391); /* 64 */

   i_state[0] += a;
   i_state[1] += b;
   i_state[2] += c;
   i_state[3] += d;

   /** memset (i_digest, 0x0, sizeof (*i_digest)); */
}

static void v_encode(unsigned int *i_digest, unsigned char *c_digest, unsigned int i_length) /* Encodes input (unsigned int) into output (unsigned char). */
{
   int i, j;
   for (i = 0, j = 0; j < i_length; i++, j += 4) /* Assumes length is multiple of 4. */
   { 
      c_digest[j]    = (unsigned char)(i_digest[i] & 0xff);
      c_digest[j + 1] = (unsigned char)((i_digest[i] >> 8) & 0xff);
      c_digest[j + 2] = (unsigned char)((i_digest[i] >> 16) & 0xff);
      c_digest[j + 3] = (unsigned char)((i_digest[i] >> 24) & 0xff);
   }
}

static void v_decode(const unsigned char *c_digest, unsigned int *i_digest, unsigned int i_length) /* Decodes input (unsigned char) into output (unsigned int). */
{
   int i, j;
   for (i = 0, j = 0; j < i_length; i++, j += 4) /* Assumes length is multiple of 4. */
   {
      i_digest[i] = ((unsigned int)c_digest[j]) |
               (((unsigned int)c_digest[j + 1]) << 8) |
               (((unsigned int)c_digest[j + 2]) << 16) |
               (((unsigned int)c_digest[j + 3]) << 24);
   }
}
