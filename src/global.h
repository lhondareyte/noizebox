/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2021-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_ENV_H__
#define __NOIZEBOX_ENV_H__

#include <fluidsynth.h>
#include <stdint.h>

#define  STD         0x01
#define  EWI         0x02
#define  WX5         0x03

#define  BREATH_A    0x01
#define  BREATH_B    0x02
#define  BREATH_C    0x03
#define  BREATH_D    0x04
#define  BREATH_E    0x05
#define  BREATH_F    0x06

#ifndef INVALID
#define  INVALID  -1
#endif
#ifndef FALSE
#define  FALSE  0
#endif
#ifndef TRUE
#define  TRUE  1
#endif

extern char * NZDIR;

extern char CONF_DB[256];

  /* fonts */
extern char FONT_DB[256];
extern int startup_font;          // font id store in noizebox.conf
extern int font_key_offset;
extern char current_font_name[];
extern char current_font_path[];
extern char ramdisk_font_path[];
extern volatile int current_font; // current font id

extern int NZ_pitch_detune;

extern int NZ_pcm_volume_left;
extern int NZ_pcm_volume_right;

extern volatile int NZ_midi_mode;
extern volatile int NZ_breath_curve;
extern const uint8_t * p_current_curve;

extern fluid_synth_t* synth;
extern fluid_midi_router_t* router;
extern fluid_settings_t* synth_settings;
extern fluid_audio_driver_t* synth_audio_driver;
extern fluid_midi_driver_t* synth_midi_driver;

#endif // NOIZEBOX_ENV_H__
