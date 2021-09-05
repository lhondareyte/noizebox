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

#ifndef BREATH_MSB
#define BREATH_MSB 0x02
#endif

#ifndef BREATH_LSB
#define BREATH_LSB 0x22
#endif

fluid_synth_t* synth;
fluid_settings_t* synth_settings;
fluid_audio_driver_t* synth_audio_driver;
fluid_midi_driver_t* synth_midi_driver;
int fluid_send_midi_event(void *, fluid_midi_event_t*);

#if defined ( __LEGACY_MIDI_PARSER__ )
  #include <pthread.h>
#else

#if defined (__WITH_JACK__)
#define MIDI_DRIVER     "jack"
#define AUDIO_DRIVER    "jack"
#else
#define MIDI_DRIVER     "oss"
#define AUDIO_DRIVER    "oss"
#endif

int velocity;
int control=0;	// Control change number
int cvalue=0;	// Control change value
fluid_midi_driver_t* synth_midi_driver;

#endif

#endif /* __NOIZEBOX_SYNTH_H__ */
