/*
 * gcc-md5sum.h
 *
 * Copyright(C) 2025   MT
 *
 * A standalone MD5 implementation in ANSI C based on RFC 1321.
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
 * 
 * TODO:                
 * 
 */

struct md5context{
   unsigned int state[4];      /* state (ABCD) */
   unsigned int count[2];      /* number of bits, modulo 2^64 (lsb first) */
   unsigned char buffer[64];
};

/* F, G, H and I are basic MD5 functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* Transformation functions for rounds 1...4 */
#define FF(a, b, c, d, x, s, ac) \
{ \
   (a) += F ((b), (c), (d)) + (x) + (unsigned int)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
}

#define GG(a, b, c, d, x, s, ac) \
{ \
   (a) += G ((b), (c), (d)) + (x) + (unsigned int)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
}

#define HH(a, b, c, d, x, s, ac) \
{ \
   (a) += H ((b), (c), (d)) + (x) + (unsigned int)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
}

#define II(a, b, c, d, x, s, ac) \
{ \
   (a) += I ((b), (c), (d)) + (x) + (unsigned int)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
}


void v_init(struct md5context *t_context);
void v_update(struct md5context *t_context, const unsigned char *c_digest, unsigned int inputLen);
void v_final(struct md5context *t_context, unsigned char c_digest[16]);

static void v_transform(unsigned int i_state[4], const unsigned char c_block[64]);
static void v_encode(unsigned int *i_digest, unsigned char *c_digest, unsigned int i_length);
static void v_decode(const unsigned char *c_digest, unsigned int *i_digest, unsigned int i_length);
