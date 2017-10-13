/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#ifndef __NOIZEBOX_H__
  #define __NOIZEBOX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <curses.h>
#include <fluidsynth.h>

int current_font;
volatile int current_font_id;
char current_font_name[14];
char current_font_path[256];
char ramdisk_font_path[256];
int font_key_offset;
int prev_v;			// previous volume

int NZ_pitch_detune;

char * NZDIR;
char FONT_DB[256];
char CONF_DB[256];

int channel_preset;
int vol,NZ_pcm_volume_left,NZ_pcm_volume_right;

#define INVALID -1

#if defined (__SPDIF_ADAPTER__)
int NZ_audio_device;
#endif

/*
	Gestion des windcontrollers
*/
volatile int NZ_midi_mode;
#define STD			0x01
#define EWI			0x02
#define WX5			0x03
#define MAX_MIDI_MODE	3

volatile int NZ_breath_curve;
const int * p_current_curve;
#define BREATH_A		0x01
#define BREATH_B		0x02
#define BREATH_C		0x03
#define BREATH_D		0x04
#define BREATH_E		0x05
#define BREATH_F		0x06
#define MAX_BREATH_MODE		6

fluid_synth_t* synth;
fluid_midi_router_t* router;
fluid_settings_t* synth_settings;
fluid_audio_driver_t* synth_audio_driver;

/*
 * Variable NZPATH
 */

char * env;	

#ifndef __FLUIDSYNTH_MIDI_DRIVER__
  #include <pthread.h>
#else
fluid_midi_driver_t* synth_midi_driver;
#endif

#endif /* __NOIZEBOX_H__ */
