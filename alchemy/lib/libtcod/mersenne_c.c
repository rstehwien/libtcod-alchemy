/*
* libtcod 1.4.1
* Copyright (c) 2008,2009 J.C.Wilk
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of J.C.Wilk may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY J.C.WILK ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL J.C.WILK BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "libtcod.h"

/* mersenne twister toolkit */
typedef struct {
	uint32 mt[624];
	int cur_mt;
} mersenne_data_t;

static TCOD_random_t instance=NULL;
static float rand_div=1.0f/(float)(0xffffffff);

/* initialize the mersenne twister array */
static void mt_init(uint32 seed, uint32 mt[624] )
{
	int i;
    mt[0]= seed;
    for (i=1; i<624; i++) {
        mt[i] = (1812433253 * (mt[i-1] ^ (mt[i-1] >> 30)) + i);
    }
}

/* get the next random value from the mersenne twister array */
static uint32 mt_rand(uint32 mt[624], int *cur_mt)
{
#define MT_HIGH_BIT 0x80000000UL
#define MT_LOW_BITS 0x7fffffffUL
    uint32 y;

    if (*cur_mt == 624) {
    	/* our 624 sequence is finished. generate a new one */
        int i;

        for (i=0;i<623;i++) {
            y = ( mt[i] & MT_HIGH_BIT ) | ( mt[i+1] & MT_LOW_BITS );
            if ( y & 1 ) {
            	// odd y
	            mt[i] = mt[ (i + 397) % 624 ] ^ (y >> 1) ^ 2567483615UL;
			} else {
				// even y
	            mt[i] = mt[ (i + 397) % 624 ] ^ (y >> 1);
			}
        }
        y = ( mt[623] & MT_HIGH_BIT ) | ( mt[0] & MT_LOW_BITS );
        if ( y & 1 ) {
        	// odd y
	        mt[623] = mt[396] ^ (y >> 1) ^ 2567483615UL;
		} else {
	        mt[623] = mt[396] ^ (y >> 1);
		}

        *cur_mt = 0;
    }

    y = mt[(*cur_mt)++];
    y ^= (y >> 11);
    y ^= (y << 7) & 2636928640UL;
    y ^= (y << 15) & 4022730752UL;
    y ^= (y >> 18);
    return y;
}

/* get a random float between 0 and 1 */
static float frandom01(mersenne_data_t *r) {
	return (float)mt_rand(r->mt,&r->cur_mt)*rand_div;
}

/* string hashing function */
static uint32 hash(const char *data,int len) {
	uint32 hash = 0;
	uint32 x;
	int i;
	for(i = 0; i < len; data++, i++) {
		hash = (hash << 4) + (*data);
		if((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return (hash & 0x7FFFFFFF);
}

TCOD_random_t TCOD_random_new() {
	mersenne_data_t *r = (mersenne_data_t *)calloc(sizeof(mersenne_data_t),1);
	r->cur_mt=624;
	mt_init((uint32)time(NULL),r->mt);
	return (TCOD_random_t)r;
}

TCOD_random_t TCOD_random_get_instance() {
	if (! instance ) {
		instance=TCOD_random_new();
	}
	return instance;
}

TCOD_random_t TCOD_random_new_from_seed(uint32 seed) {
	mersenne_data_t *r = (mersenne_data_t *)calloc(sizeof(mersenne_data_t),1);
	r->cur_mt=624;
	mt_init(seed,r->mt);
	return (TCOD_random_t)r;
}

int TCOD_random_get_int(TCOD_random_t mersenne, int min, int max) {
	mersenne_data_t *r;
	int delta;
	if (max==min) return min;
	else if (max < min) {
		int tmp=max;
		max=min;
		min=tmp;
	}
	if (!mersenne) mersenne=TCOD_random_get_instance();
	r=(mersenne_data_t *)mersenne;
	delta = max - min + 1;
	return ( mt_rand(r->mt,&r->cur_mt)  % delta ) + min;
}

float TCOD_random_get_float(TCOD_random_t mersenne,float min, float max) {
	mersenne_data_t *r;
	float delta,f;
	if (max==min) return min;
	else if (max < min) {
		float tmp=max;
		max=min;
		min=tmp;
	}
	if (!mersenne) mersenne=TCOD_random_get_instance();
	r=(mersenne_data_t *)mersenne;
	delta = max - min;
	f = delta * frandom01(r);
	return min + f;
}

int TCOD_random_get_int_from_byte_array(int min, int max, const char *data,int len) {
	static uint32 mt_seed[624];
	static int cur_mt_seed;
	static uint32 mt_seed_hash=0;
	static uint32 mt_seed_rand=0;
	uint32 h;
	if (max==min) return min;
	else if (max < min) {
		int tmp=max;
		max=min;
		min=tmp;
	}
	h=hash(data,len);
	if ( h != mt_seed_hash ) {
		mt_seed_hash=h;
		mt_init(h,mt_seed);
		cur_mt_seed=624;
		mt_seed_rand = (mt_rand(mt_seed,&cur_mt_seed) % (max-min+1)) + min;
	}
	return mt_seed_rand;
}

void TCOD_random_delete(TCOD_random_t mersenne) {
	free(mersenne);
}
TCOD_random_t TCOD_random_save(TCOD_random_t mersenne) {
	mersenne_data_t *ret=(mersenne_data_t *)malloc(sizeof(mersenne_data_t));
	if (!mersenne) mersenne=TCOD_random_get_instance();
	memcpy(ret,mersenne,sizeof(mersenne_data_t));
	return (TCOD_random_t)ret;
}

void TCOD_random_restore(TCOD_random_t mersenne, TCOD_random_t backup) {
	if (!mersenne) mersenne=TCOD_random_get_instance();
	memcpy(mersenne,backup,sizeof(mersenne_data_t));
}

