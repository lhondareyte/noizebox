/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2025, Luc Hondareyte
 * All rights reserved.
 * 
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#ifdef __linux__
#include <stdint.h>
#endif
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <unistd.h>

#include "global.h"

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
	ioctl(mixer, MIXER_READ(SOUND_MIXER_PCM), &v);
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
	ioctl(mixer, SOUND_MIXER_WRITE_VOLUME, &v);
	v = NZ_pcm_volume_right;
	v = (v << 8 ) + NZ_pcm_volume_left;
	return ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
}

#ifdef __NOIZEBOX_DEBUG__
void NZ_maximize_pcm_volume(void)
{
	int v = 0x6464;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
}

void NZ_mute_pcm_volume(void)
{
	int v = 0x0000;
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
}
#endif

void NZ_increment_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ((l < NOIZEBOX_MAX_LEVEL) && (r < NOIZEBOX_MAX_LEVEL)) {
		prev_v = v;
		v += 0x0101;
		ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
	}
}

void NZ_decrement_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ((l > NOIZEBOX_MIN_LEVEL) && (r > NOIZEBOX_MIN_LEVEL)) {
		prev_v = v;
		v -= 0x0101;
		ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
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
	if (r > l) {
		if (l < NOIZEBOX_MAX_LEVEL)
			v += 0x0001;
	}
	else {
		if (r > NOIZEBOX_MIN_LEVEL)
			v -= 0x0100;
	}
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
}

/* Down to the left is same as up to the right */
void NZ_increment_left_pcm_volume(void)
{
	int v, r, l;
	v = NZ_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if (l > r) {
		if (r < NOIZEBOX_MAX_LEVEL)
			v += 0x0100;
	}
	else {
		if (l > NOIZEBOX_MIN_LEVEL)
			v -= 0x0001;
	}
	ioctl(mixer, MIXER_WRITE(SOUND_MIXER_PCM), &v);
}

