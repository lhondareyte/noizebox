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

/*
 * MIDI Controllers
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

#if defined (__LEGACY_MIDI_PARSER__)
  #include <pthread.h>
#else
fluid_midi_driver_t* synth_midi_driver;
#endif

#endif /* __NOIZEBOX_H__ */
