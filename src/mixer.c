/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include "noizebox.h"


#define noizebox_shutdown_mixer() close()
#define NOIZEBOX_MIN_LEVEL	0
#define NOIZEBOX_MAX_LEVEL	100

int noizebox_get_pcm_volume(void)
{
	int v;
	ioctl(noizebox_mixer, MIXER_READ(SOUND_MIXER_PCM),&v);
	return(v);
}

int noizebox_init_mixer(void)
{
	int16_t v;
	v = NOIZEBOX_MAX_LEVEL;
	v = (v << 8) + NOIZEBOX_MAX_LEVEL;
	if ((noizebox_mixer = open("/dev/mixer",O_RDWR|O_NONBLOCK)) == -1) 
	{
		fprintf(stderr,"Error while opening Mixer device: /dev/mixer: %s\n",strerror(errno));
		return -1;
	}
	ioctl(noizebox_mixer, SOUND_MIXER_WRITE_VOLUME,&v);
	v = NZ_pcm_volume_left;
	v = (v << 8 ) + NZ_pcm_volume_right;
	return ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

#ifdef __NOIZEBOX_DEBUG__
void noizebox_maximize_pcm_volume(void)
{
	int v=0x6464;
	ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

void noizebox_mute_pcm_volume(void)
{
	int v=0x0000;
	ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}
#endif

void noizebox_increment_pcm_volume(void)
{
	int v, r, l;
	v = noizebox_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if (l < NOIZEBOX_MAX_LEVEL && r < NOIZEBOX_MAX_LEVEL )
	{
		prev_v=v;
		v+= 0x0101;
		ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
	}
}

void noizebox_decrement_pcm_volume(void)
{
	int v, r, l;
	v = noizebox_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if (l > NOIZEBOX_MIN_LEVEL && r > NOIZEBOX_MIN_LEVEL )
	{
		prev_v=v;
		v-= 0x0101;
		ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
	}
}

/* Gestion de la balance */
/* Augmenter à droite revient à diminuer à gauche */
void noizebox_increment_right_pcm_volume(void)
{
	int v, r, l;
	v = noizebox_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ( r > l ) 
	{
		if ( l < NOIZEBOX_MAX_LEVEL) v+=0x0001;
	}
	else 
	{
		if ( r > NOIZEBOX_MIN_LEVEL) v-=0x0100;
	}
	ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

/* Augmenter à gauche revient à diminuer à droite */
void noizebox_increment_left_pcm_volume(void)
{
	int v, r, l;
	v = noizebox_get_pcm_volume();
	l = v & 0xFF;
	r = v >> 8;
	if ( l > r ) 
	{
		if ( r < NOIZEBOX_MAX_LEVEL) v+=0x0100;
	}
	else 
	{
		if ( l > NOIZEBOX_MIN_LEVEL) v-=0x0001;
	}
	ioctl(noizebox_mixer, MIXER_WRITE(SOUND_MIXER_PCM),&v);
}

#ifdef __NOIZEBOX_AUX_ENABLE__

void noizebox_increment_line_volume(void)
{
	ioctl(noizebox_mixer, SOUND_MIXER_WRITE_PCM,&noizebox_pcm_volume);
}

void noizebox_decrement_line_volume(void)
{
	ioctl(noizebox_mixer, SOUND_MIXER_WRITE_PCM,&noizebox_pcm_volume);
}
#endif
