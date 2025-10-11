/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2025, Luc Hondareyte
 * All rights reserved.
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

int startup_font = 1;
volatile int current_font = -1;
char current_font_name[14];
char current_font_path[256];
char ramdisk_font_path[256];
int font_key_offset;

int NZ_pitch_detune;

char * NZDIR;
char FONT_DB[256];
char CONF_DB[256];

int channel_preset;
int vol,NZ_pcm_volume_left,NZ_pcm_volume_right;

volatile int NZ_midi_mode;
volatile int NZ_breath_curve;
const uint8_t * p_current_curve;

fluid_synth_t* synth;
fluid_midi_router_t* router;
fluid_settings_t* synth_settings;
fluid_audio_driver_t* synth_audio_driver;

#if defined (__LEGACY_MIDI_PARSER__)
#include <pthread.h>
#else
fluid_midi_driver_t* synth_midi_driver;
#endif

#endif /* __NOIZEBOX_H__ */
