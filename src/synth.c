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

#include "noizebox.h"
#include "functions.h"
#ifndef	REALTIME
 #define	REALTIME 1
#endif
#include "synth.h"

#if !defined (__LEGACY_MIDI_PARSER__)

int fluid_send_midi_event(void * data, fluid_midi_event_t* event) {
	int chan = fluid_midi_event_get_channel(event);
	switch (fluid_midi_event_get_type(event)) {
		case NOTE_OFF:
			fluid_synth_noteoff(synth, chan, fluid_midi_event_get_key(event));
			break;

		case NOTE_ON:
			velocity=fluid_midi_event_get_velocity(event);
			fluid_synth_noteon(synth, chan, fluid_midi_event_get_key(event),
					velocity);
			break;

		case CONTROL_CHANGE:
			fluid_synth_cc(synth, chan, fluid_midi_event_get_control(event),
					fluid_midi_event_get_value(event));
			break;

		case PROGRAM_CHANGE:
			fluid_synth_program_change(synth, chan, fluid_midi_event_get_program(event));
			break;

		case PITCH_BEND:
			fluid_synth_pitch_bend(synth, chan, fluid_midi_event_get_pitch(event));
			break;

		case CHANNEL_PRESSURE:
			fluid_synth_channel_pressure(synth, chan, fluid_midi_event_get_value(event));
			break;

		case MIDI_SYSTEM_RESET:
			fluid_synth_system_reset(synth);
			break;

		default:  
			return FLUID_FAILED;

	}
	return 0;
}
#endif

void NZ_delete_synth(void) {
	delete_fluid_synth(synth);
	delete_fluid_settings(synth_settings);
#ifdef __FLUIDSYNTH_MIDI_DRIVER__
	delete_fluid_midi_driver(synth_midi_driver);
#endif
}

void NZ_create_synth(void) {
	synth_settings = new_fluid_settings();
	synth = new_fluid_synth(synth_settings);
#if defined (__SPDIF_ADAPTER__)
	switch (NZ_audio_device) {
		case 0x00:
			fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp0");
			break;
		case 0x01:
			fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp1");
			break;
		case 0x02:
			fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp2");
			break;
		case 0x03:
			fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp3");
			break;
		default:
			fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp");
			break;
	}
#else
	fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp");
#endif

#ifdef __NOIZEBOX_DEBUG__
	fluid_settings_setint(synth_settings, "synth.verbose", TRUE);
#endif
	fluid_settings_setnum(synth_settings, "synth.polyphony", 64);
	fluid_settings_setnum(synth_settings, "synth.gain", 1.0);
	fluid_settings_setstr(synth_settings, "audio.driver", "oss");
	synth_audio_driver = new_fluid_audio_driver(synth_settings, synth);
	NZ_init_mixer();

#if !defined (__LEGACY_MIDI_PARSER__)
	fluid_settings_setstr(synth_settings, "midi.oss.device", "/dev/umidi0.0");
	synth_midi_driver = new_fluid_midi_driver(synth_settings, fluid_send_midi_event, NULL);
#endif
#ifndef __WITH_SQLITE__
	current_font=0;
	current_font_id=fluid_synth_sfload(synth, NZ_bank[current_font].file, TRUE);
#else
	NZ_load_bank();
	current_font_id=0;
	NZ_load_font(current_font);
#endif
}

void NZ_synth_detune(int p) {
	int key;
	double pitch;
	for (key=0; key<=127; key++) {
		pitch = ((key * 10) + (double)p ) * 10;
		fluid_synth_tune_notes(synth,1,1,1,&key,&pitch,TRUE);
	}
}

