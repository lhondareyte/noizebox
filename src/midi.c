/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#ifndef __FLUIDSYNTH_MIDI_DRIVER__
#define __FLUIDSYNTH_MIDI_DRIVER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "noizebox.h"
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
	// Traitement Status Byte
	if (bit_is_set(buffer,7))
	{
		// Real Time messages
		if ( buffer >= MIDI_SYSEX_MSG )
		{
			switch (buffer)
			{
			case MIDI_SYSEX_MSG:
				rsbuff=status;
				status = buffer;
				next=MIDI_SYSEX_MSG;
				break;
				
			case MIDI_QUARTER_MSG:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
				
			case MIDI_SONGPOS_MSG:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
			
			case MIDI_SONGSEL_MSG:
				rsbuff=status;
				status = buffer;
				next = MIDI_DATA1;
				break;
			
#if defined ( __MIDI_RESET_ENABLE__ )
			case MIDI_RESET_MSG:	
				fluid_synth_system_reset (synth);
				ready = MIDI_TRUE;
				break;
#endif

			case MIDI_EOX_MSG:
				next=MIDI_UNKNOW_MSG;
				ready=MIDI_TRUE;
				break;

			default:
				break;
			}
		}		
		else 
		{
			rsbuff=status;		// Stockage du status précedent pour la gestion
						// du Running status
			status=buffer&0xF0;
			channel=buffer&0x0F;
			next=MIDI_DATA1;
			ready=MIDI_FALSE;
		}
		// System Common Messages

	}
	// Traitement des DATA Bytes
	else
	{
		switch (next)
		{
			case MIDI_DATA1: 
				data1=buffer;
				next=MIDI_UNKNOW_MSG;
				switch (status)
				{
					case MIDI_PROGCH_MSG:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_CHANAF_MSG:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_QUARTER_MSG:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_SONGPOS_MSG:
						fluid_synth_program_change(synth, channel, data1);
						break;
					case MIDI_SONGSEL_MSG:
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
				switch (status)
				{
					case MIDI_NOTEON_MSG : 
						k = data1 + font_key_offset ;
						if ( k < 0 || k > 127 ) break ;
						else data1 = k;
						if ( data2 == 0 ) 
						{
							fluid_synth_noteoff(synth, channel, data1);
						}
						else
						{
							if ( NZ_midi_mode == WX5 ) data2=100;
							fluid_synth_noteon(synth, channel, data1,  data2);
						}
						break;

					case MIDI_NOTOFF_MSG : 
						k = data1 + font_key_offset ;
						if ( k < 0 || k > 127 ) break ;
						else data1 = k;
						fluid_synth_noteoff(synth, channel, data1);
						break;

					case MIDI_POLYAF_MSG : 
						break;

					case MIDI_CTRLCHG_MSG : 
						if ( NZ_midi_mode == EWI || NZ_midi_mode == WX5 ) 
						{
							if ( data1 == 2 || data1 == 34 )
							{
							/* Mappage du breath control sur le volume pour les EWIs */
								data1+=5;
							/* Prise en compte de la courbe de reponse du souffle */
								data2= *(p_current_curve + data2);
							}
						}
						fluid_synth_cc(synth, channel, data1, data2);
						break;

					case MIDI_PITCHB_MSG : 
						// Attention, mauvaise analyse de la fonction picthbend
						fluid_synth_pitch_bend(synth, channel, data1);
						break;

					case MIDI_SONGPOS_MSG :
						break;

					// Arrivé ici, on a une c. dans le potage.
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

			case MIDI_SYSEX_MSG :
				#if defined (MIDI_MANUFACTURER_ID)

				#endif
				next=MIDI_SYSEX_MSG;
				break;

			// Gestion du running status
			case MIDI_UNKNOW_MSG :
				if (rsbuff < MIDI_SYSEX_MSG)
				{
					status=rsbuff;
					data1=buffer;
					next=MIDI_DATA2;
					if (rsbuff == MIDI_PROGCH_MSG || rsbuff == MIDI_CHANAF_MSG )
					{
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
	while (( fp=fopen(dev,"r")) == NULL ) 
	{
		fprintf(stderr,"Error: %s: %s (will trying later)\n",dev,strerror(errno));
		sleep(5);
	}
	fprintf(stderr,"Opening %s OK\n", dev);
	while (!feof(fp))
	{
		c = fgetc(fp);
		NZ_midi_analyze(c);
	}
	return(0);
}

#endif   // __FLUIDSYNTH_MIDI_DRIVER__
