/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#if defined (__LEGACY_MIDI_PARSER__) 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <stdint.h>
#endif

#include "global.h"
#include "midi.h"
#include "breath.h"

#ifndef FALSE
 #define FALSE 0
#endif
#ifndef TRUE
 #define TRUE 1
#endif


#ifndef bit_is_set
 #define bit_is_set(var,pos) ((var) & (1<<(pos)))
#endif

volatile uint8_t buffer;
volatile uint8_t status;
volatile uint8_t channel;       // MIDI channel from current message
volatile uint8_t next;          // Next value expected
volatile int     k;
volatile uint8_t data1;
volatile uint8_t data2;
volatile uint8_t ready;         // Message ready or not
volatile uint8_t rsbuff;        // Running status buffer
uint8_t activechannel;          // MIDI Active channel

void NZ_midi_analyze(uint8_t v)
{
	buffer=v;
	// Check Status Byte
	if (bit_is_set(buffer,7)) {
		// Real Time messages
		if ( buffer >= MIDI_SYSEX ) {
			switch (buffer) {
				case MIDI_SYSEX:
				rsbuff=status;
				status = buffer;
				next=MIDI_SYSEX;
				break;
				
				case MIDI_TIME_CODE:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
				
				case MIDI_SONG_POSITION:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
			
				case MIDI_SONG_SELECT:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
			
#if defined ( __MIDI_RESET_ENABLE__ )
				case MIDI_SYSTEM_RESET:
				fluid_synth_system_reset (synth);
				ready = MIDI_TRUE;
				break;
#endif

			case MIDI_EOX:
				next=MIDI_UNKNOWN;
				ready=MIDI_TRUE;
				break;

			default:
				break;
			}
		}		
		else {
			rsbuff=status;		// Store previous status for
						// Running status
			status=buffer&0xF0;
			channel=buffer&0x0F;
			next=MIDI_DATA1;
			ready=MIDI_FALSE;
		}
		// System Common Messages

	}
	// Traitement des DATA Bytes
	else {
		switch (next) {
			case MIDI_DATA1: 
				data1=buffer;
				next=MIDI_UNKNOWN;
				switch (status) {
					case MIDI_PROGRAM_CHANGE:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_CHANNEL_PRESSURE:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_TIME_CODE:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_SONG_POSITION:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_SONG_SELECT:
						fluid_synth_program_change(synth, channel, data1);
						break;
					default:
						next=MIDI_DATA2;
						ready=MIDI_TRUE;
						break;
				}
				break;
						
			case MIDI_DATA2: 
				data2=buffer;
				switch (status) {
					case MIDI_NOTE_ON:
						k = data1 + font_key_offset ;
						if ( k < 0 || k > 127 ) break ;
						else data1 = k;
						if ( data2 == 0 )
							fluid_synth_noteoff(synth, channel, data1);
						else {
							if ( NZ_midi_mode == WX5 ) data2=100;
							fluid_synth_noteon(synth, channel, data1,  data2);
						}
						break;

					case MIDI_NOTE_OFF: 
						k = data1 + font_key_offset ;
						if ( k < 0 || k > 127 ) break ;
						else data1 = k;
						fluid_synth_noteoff(synth, channel, data1);
						break;

					case MIDI_KEY_PRESSURE: 
						break;

					case MIDI_CONTROL_CHANGE: 
						if ( NZ_midi_mode == EWI || NZ_midi_mode == WX5 ) {
							if ( data1 == MIDI_BREATH_CTRL || data1 == MIDI_BREATH_FINE_CTRL ) {
							/* Map breath control on volume for EWIs */
								data1+=5;
							/* Translate current breath curve */
								data2= *(p_current_curve + data2);
							}
						}
						fluid_synth_cc(synth, channel, data1, data2);
						break;

					case MIDI_PITCH_BEND: 
						// Warning, possible problem with pitchbend
						fluid_synth_pitch_bend(synth, channel, data1);
						break;

					case MIDI_SONG_POSITION:
						break;

					// You should not arrive here
					default: 
						status=MIDI_UNKNOWN;
						next=MIDI_UNKNOWN;
						ready=FALSE;
						fluid_synth_system_reset(synth);
						break;
				}
				next=MIDI_UNKNOWN;
				ready=TRUE;
				break;

			case MIDI_SYSEX:
				#if defined (MIDI_MANUFACTURER_ID)

				#endif
				next=MIDI_SYSEX;
				break;

			// Running status
			case MIDI_UNKNOWN :
				if (rsbuff < MIDI_SYSEX) {
					status=rsbuff;
					data1=buffer;
					next=MIDI_DATA2;
					if (rsbuff == MIDI_PROGRAM_CHANGE || rsbuff == MIDI_CHANNEL_PRESSURE ) {
						next=MIDI_UNKNOWN;
						ready=MIDI_TRUE;
					}
				}
				break;

			default:
				MIDI_Init(channel);
				#if defined (__MIDI_RESET_ON_ERROR__)
				#warning "MIDI_RESET_ON_ERROR is defined"
				exit (0);
				#endif
				break;
		}
	}
}

int *NZ_midi_read( char *dev )
{
	FILE *fp;
	uint8_t c;
	while (( fp=fopen(dev,"r")) == NULL ) {
#ifdef __NOIZEBOX_DEBUG__
		fprintf(stderr,"Warning: %s: %s\n",dev,strerror(errno));
#endif
		return 0;
	}
#ifdef __NOIZEBOX_DEBUG__
	fprintf(stderr,"Opening %s OK\n", dev);
#endif
	while (!feof(fp)) {
		c = fgetc(fp);
		NZ_midi_analyze(c);
	}
	fclose(fp);
	return 0;
}

#endif   // __LEGACY_MIDI_PARSER__
