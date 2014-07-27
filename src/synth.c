/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include "noizebox.h"
#include "functions.h"
#ifndef	REALTIME
#define	REALTIME 1
#endif

#ifdef __FLUIDSYNTH_MIDI_DRIVER__

int fluid_send_midi_event(void * data, fluid_midi_event_t* event)
{
	//int chan=0x03;
	int chan = fluid_midi_event_get_channel(event);
	//if ( chan != noizebox_midi_channel ) return 0;

	switch (fluid_midi_event_get_type(event)) 
	{
		case NOTE_OFF:
			fluid_synth_noteoff(synth, chan, fluid_midi_event_get_key(event));
			break;

		case NOTE_ON:
			int velocity=fluid_midi_event_get_velocity(event);
			if ( velocity <= noizebox_noteon_minimum ) 
				velocity=noizebox_noteon_minimum;
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

void noizebox_delete_synth(void)
{
	delete_fluid_synth(synth);
	delete_fluid_settings(synth_settings);
#ifdef __FLUIDSYNTH_MIDI_DRIVER__
	delete_fluid_midi_driver(synth_midi_driver);
#endif
}

void noizebox_create_synth(void)
{
	noizebox_pitch=0;
	synth_settings = new_fluid_settings();
	synth = new_fluid_synth(synth_settings);
	fluid_settings_setstr(synth_settings, "audio.driver", "oss");
	fluid_settings_setstr(synth_settings, "synth.gain", "5.00");

	synth_audio_driver = new_fluid_audio_driver(synth_settings, synth);
	noizebox_init_mixer();

#ifdef __FLUIDSYNTH_MIDI_DRIVER__
	fluid_settings_setstr(synth_settings, "midi.driver", "oss");
	fluid_settings_setstr(synth_settings, "midi.oss.device", "/dev/umidi0.0");
	synth_midi_driver = new_fluid_midi_driver(synth_settings, fluid_send_midi_event, NULL);
#endif
#ifndef __WITH_SQLITE__
	current_font=0;
	current_font_id=fluid_synth_sfload(synth, noizebox_bank[current_font].file, 1);
#else
	noizebox_load_bank();
	current_font_id=0;
	noizebox_load_font(current_font);
#endif
}

void noizebox_synth_detune(int p)
{
	int key;
	double pitch;
	NZ_pitch_detune += p;
	for (key=0; key<=127; key++)
	{
		pitch = ((key * 10) + (double)NZ_pitch_detune ) * 10;
		fluid_synth_tune_notes(synth,1,1,1,&key,&pitch,TRUE);
	}
}
