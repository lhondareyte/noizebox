/*
 * Copyright (c)2013-2017, Luc Hondareyte
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification,  are permitted provided that the following conditions
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

#if defined (__LEGACY_MIDI_PARSER__) 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
				next=MIDI_SYSEX_MSG;
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
				next=MIDI_UNKNOW_MSG;
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
				next=MIDI_UNKNOW_MSG;
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
							if ( data1 == MIDI_BREATH_CONTROL || data1 == MIDI_BREATH_FINE_CONTROL ) {
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
						status=MIDI_UNKNOW_MSG;
						next=MIDI_UNKNOW_MSG;
						ready=FALSE;
						fluid_synth_system_reset(synth);
						break;
				}
				next=MIDI_UNKNOW_MSG;
				ready=TRUE;
				break;

			case MIDI_SYSEX:
				#if defined (MIDI_MANUFACTURER_ID)

				#endif
				next=MIDI_SYSEX;
				break;

			// Running status
			case MIDI_UNKNOW_MSG :
				if (rsbuff < MIDI_SYSEX) {
					status=rsbuff;
					data1=buffer;
					next=MIDI_DATA2;
					if (rsbuff == MIDI_PROGRAM_CHANGE || rsbuff == MIDI_CHANNEL_PRESSURE ) {
						next=MIDI_UNKNOW_MSG;
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
		return(0);
	}
#ifdef __NOIZEBOX_DEBUG__
	fprintf(stderr,"Opening %s OK\n", dev);
#endif
	while (!feof(fp)) {
		c = fgetc(fp);
		NZ_midi_analyze(c);
	}
	fclose(fp);
	return(0);
}

#endif   // __LEGACY_MIDI_PARSER__
