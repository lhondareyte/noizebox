/*
 * Copyright (c)2021, Luc Hondareyte
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

#ifndef __NOIZEBOX_ENV_H__
#define __NOIZEBOX_ENV_H__

#include <fluidsynth.h>

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
extern int current_font;
extern int font_key_offset;
extern char current_font_name[];
extern char current_font_path[];
extern char ramdisk_font_path[];
extern volatile int current_font_id;

extern int NZ_pitch_detune;

extern int NZ_pcm_volume_left;
extern int NZ_pcm_volume_right;

extern volatile int NZ_midi_mode;
extern volatile int NZ_breath_curve;
extern const int * p_current_curve;

extern fluid_synth_t* synth;
extern fluid_midi_router_t* router;
extern fluid_settings_t* synth_settings;
extern fluid_audio_driver_t* synth_audio_driver;
extern fluid_midi_driver_t* synth_midi_driver;

#endif // NOIZEBOX_ENV_H__
