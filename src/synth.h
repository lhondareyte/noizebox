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

#ifndef __NOIZEBOX_SYNTH_H__
#define __NOIZEBOX_SYNTH_H__

#include <fluidsynth.h>

fluid_synth_t* synth;
fluid_settings_t* synth_settings;
fluid_audio_driver_t* synth_audio_driver;

#ifndef __FLUIDSYNTH_MIDI_DRIVER__
  #include <pthread.h>
#else
fluid_midi_driver_t* synth_midi_driver;
#endif

  /* channel messages */
#define  NOTE_OFF 	 	0x80
#define  NOTE_ON 	 	0x90
#define  KEY_PRESSURE 	 	0xa0
#define  CONTROL_CHANGE 	0xb0
#define  PROGRAM_CHANGE 	0xc0
#define  CHANNEL_PRESSURE 	0xd0
#define  PITCH_BEND 	 	0xe0
  /* system exclusive */
#define  MIDI_SYSEX 	 	0xf0
/* system common - never in midi files */
#define  MIDI_TIME_CODE 	0xf1
#define  MIDI_SONG_POSITION 	0xf2
#define  MIDI_SONG_SELECT 	0xf3
#define  MIDI_TUNE_REQUEST 	0xf6
#define  MIDI_EOX 	 	0xf7
  /* system real-time - never in midi files */
#define  MIDI_SYNC 	 	0xf8
#define  MIDI_TICK 	 	0xf9
#define  MIDI_START 	 	0xfa
#define  MIDI_CONTINUE 	 	0xfb
#define  MIDI_STOP 	 	0xfc
#define  MIDI_ACTIVE_SENSING 	0xfe
#define  MIDI_SYSTEM_RESET 	0xff

#endif /* __NOIZEBOX_SYNTH_H__ */
