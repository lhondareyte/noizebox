/*
 * Copyright (c)2013-2017, Luc Hondareyte
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <unistd.h>

#include "global.h"

#define NZ_shutdown_mixer() close()
#define NOIZEBOX_MIN_LEVEL	0
#define NOIZEBOX_MAX_LEVEL	100

int mixer;
int prev_v; // previous volume level

void NZ_close_mixer(void)
{
	close(mixer);
}

int NZ_get_pcm_volume(void)
{
	int v;
	ioctl(mixer, MIXER_READ(SOUND_MIXER_PCM),&v);
	return v;
}

int NZ_init_mixer(void)
{
	int16_t v;
	v = NOIZEBOX_MAX_LEVEL;
	v = (v << 8) + NOIZEBOX_MAX_LEVEL;
	if ((mixer = open("/dev/mixer",O_RDWR|O_NONBLOCK)) == -1) {
		fprintf(stderr,"Error: Cannot open /dev/mixer: %s\n",strerror(errno));
		return -1;
	}
	ioctl(mixer, SOUND_MIXER_WRITE_VOLUME,&v);
	v = NZ_pcm_volume_right;
	v = (v << 8 ) + NZ_pcm_volume_left;
	return ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

#ifdef __NOIZEBOX_DEBUG__
void NZ_maximize_pcm_volume(void)
{
	int v=0x6464;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

void NZ_mute_pcm_volume(void)
{
	int v=0x0000;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}
#endif

void NZ_increment_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if (l < NOIZEBOX_MAX_LEVEL && r < NOIZEBOX_MAX_LEVEL ) {
		prev_v=v;
		v+= 0x0101;
		ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
	}
}

void NZ_decrement_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if (l > NOIZEBOX_MIN_LEVEL && r > NOIZEBOX_MIN_LEVEL ) {
		prev_v=v;
		v-= 0x0101;
		ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
	}
}

/* Balance management */
/* Up to the left is same as down to the right */
void NZ_increment_right_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ( r > l )
		if ( l < NOIZEBOX_MAX_LEVEL) v+=0x0001;
	else 
		if ( r > NOIZEBOX_MIN_LEVEL) v-=0x0100;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

/* Down to the left is same as up to the right */
void NZ_increment_left_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ( l > r ) 
		if ( r < NOIZEBOX_MAX_LEVEL) v+=0x0100;
	else
		if ( l > NOIZEBOX_MIN_LEVEL) v-=0x0001;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}
