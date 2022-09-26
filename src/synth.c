/*
 * Copyright (c)2013-2022, Luc Hondareyte
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

#include "bank.h"
#include "midi.h"
#include "mixer.h"
#include "global.h"

#ifndef	REALTIME
 #define	REALTIME 1
#endif

#if defined (__WITH_JACK__)
#define MIDI_DRIVER     "jack"
#define AUDIO_DRIVER    "jack"
#else
#define MIDI_DRIVER     "oss"
#define AUDIO_DRIVER    "oss"
#endif

#if !defined (__LEGACY_MIDI_PARSER__)

int velocity;
int control=0;  // Control change number
int cvalue=0;   // Control change value

int fluid_send_midi_event(void * data, fluid_midi_event_t* event)
{
	int chan = fluid_midi_event_get_channel(event);
	switch (fluid_midi_event_get_type(event)) {
		case MIDI_NOTE_OFF:
			fluid_synth_noteoff(synth, chan, fluid_midi_event_get_key(event));
			break;

		case MIDI_NOTE_ON:
			velocity=fluid_midi_event_get_velocity(event);
			fluid_synth_noteon(synth, chan, fluid_midi_event_get_key(event), velocity);
			break;

		case MIDI_CONTROL_CHANGE:
			if ( NZ_midi_mode == EWI || NZ_midi_mode == WX5 ) {
				control=fluid_midi_event_get_control(event);
				/* Map breath control on volume for EWIs */
				if ( control == BREATH_MSB )
					control+=5;
				/* Translate current breath curve */
				cvalue=fluid_midi_event_get_value(event);
				cvalue=*(p_current_curve + cvalue);

				fluid_synth_cc(synth, chan, control, cvalue);
			}
			else
				fluid_synth_cc(synth, chan, fluid_midi_event_get_control(event),
					fluid_midi_event_get_value(event));
			break;

		case MIDI_PROGRAM_CHANGE:
			fluid_synth_program_change(synth, chan, fluid_midi_event_get_program(event));
			break;

		case MIDI_PITCH_BEND:
			fluid_synth_pitch_bend(synth, chan, fluid_midi_event_get_pitch(event));
			break;

		case MIDI_CHANNEL_PRESSURE:
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

void NZ_delete_synth(void)
{
#if !defined (__LEGACY_MIDI_PARSER__)
	delete_fluid_midi_driver(synth_midi_driver);
#endif
	delete_fluid_synth(synth);
	delete_fluid_settings(synth_settings);
}

int NZ_create_synth(void)
{
	synth_settings = new_fluid_settings();
	synth = new_fluid_synth(synth_settings);

	fluid_set_log_function(FLUID_WARN, NULL, NULL);

	fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp");

#ifdef __NOIZEBOX_DEBUG__
	fluid_settings_setint(synth_settings, "synth.verbose", TRUE);
#endif
	fluid_settings_setint(synth_settings, "synth.polyphony", 64);
	fluid_settings_setnum(synth_settings, "synth.gain", 1.0);
#if defined (__WITH_JACK__)
	fluid_settings_setstr(synth_settings, "audio.jack.id", "noizebox");
	fluid_settings_setstr(synth_settings, "midi.jack.id", "noizebox");
#endif
	fluid_settings_setstr(synth_settings, "midi.driver", MIDI_DRIVER);
	fluid_settings_setstr(synth_settings, "audio.driver", AUDIO_DRIVER);
	synth_audio_driver = new_fluid_audio_driver(synth_settings, synth);
	if (! synth_audio_driver) {
		return -1;
	}

#if !defined (__LEGACY_MIDI_PARSER__) && !defined (__WITH_JACK__)
	fluid_settings_setstr(synth_settings, "midi.oss.device", "/dev/umidi0.0");
	synth_midi_driver = new_fluid_midi_driver(synth_settings, fluid_send_midi_event, NULL);
	if (! synth_midi_driver) {
		return -1;
	}

#endif
	return 0;
}

void NZ_synth_detune(int p)
{
	int key;
	double pitch;
	for (key=0; key<=127; key++) {
		pitch = ((key * 10) + (double)p ) * 10;
		fluid_synth_tune_notes(synth,1,1,1,&key,&pitch,TRUE);
	}
}

