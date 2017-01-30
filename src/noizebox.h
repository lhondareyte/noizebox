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

/*
 * MIDI stuffs
 */

  /* channel messages */
#define  NOTE_OFF 	 0x80
#define  NOTE_ON 	 0x90
#define  KEY_PRESSURE 	 0xa0
#define  CONTROL_CHANGE 	 0xb0
#define  PROGRAM_CHANGE 	 0xc0
#define  CHANNEL_PRESSURE 	 0xd0
#define  PITCH_BEND 	 0xe0
  /* system exclusive */
#define  MIDI_SYSEX 	 0xf0
/* system common - never in midi files */
#define  MIDI_TIME_CODE 	 0xf1
#define  MIDI_SONG_POSITION 	 0xf2
#define  MIDI_SONG_SELECT 	 0xf3
#define  MIDI_TUNE_REQUEST 	 0xf6
#define  MIDI_EOX 	 0xf7
  /* system real-time - never in midi files */
#define  MIDI_SYNC 	 0xf8
#define  MIDI_TICK 	 0xf9
#define  MIDI_START 	 0xfa
#define  MIDI_CONTINUE 	 0xfb
#define  MIDI_STOP 	 0xfc
#define  MIDI_ACTIVE_SENSING 	 0xfe
#define  MIDI_SYSTEM_RESET 	 0xff
  /* meta event - for midi files only */
#define  MIDI_META_EVENT 	 0xff

#endif /* __NOIZEBOX_H__ */
