/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <ncurses.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <fluidsynth.h>
#include <math.h>

#include "noizebox.h"
#include "functions.h"
#include "breath.h"

static WINDOW *screen;
int key;

void NZ_refresh(void)
{
	/* Rustine pour fonctionnement avec tinyVT */
	//usleep (25000);
	refresh();
}

void NZ_terminate_menu(void)
{
	clear();
        mvprintw(0,0,"  Shutdown in progress\n      Please wait...  ");
        NZ_refresh();
        endwin();
}

void NZ_refresh_volume(void)
{
	int16_t v;
	v=NZ_get_pcm_volume();
	if ( prev_v != v ) 
	{
		mvprintw(0,15,"V=%03d:%03d", (v & 0xFF), (v >> 8));
		NZ_refresh();
	}
}

void NZ_refresh_midi_mode(void)
{
	char * current_midi_mode_name = "";
	switch (NZ_midi_mode)
	{
		case 0x01:
			current_midi_mode_name="STD";
			break;
		case 0x02:
			current_midi_mode_name="EWI";
			break;
		case 0x03:
			current_midi_mode_name="WX5";
			break;
	}
	mvprintw(1,14,"M=%-3s",current_midi_mode_name);
	NZ_refresh();
}

#if defined (__SPDIF_ADAPTER__)
void NZ_refresh_audio_device(void)
{
	if ( NZ_audio_device == 0x01)
			mvprintw(1,20,"A=AN");
	else
			mvprintw(1,20,"A=DI");
	NZ_refresh();
}
#endif

void NZ_refresh_font_name(void)
{
	mvprintw(0,0,"P=%-13s", current_font_name);
	NZ_refresh();
}

void NZ_refresh_breath_curve()
{
	switch (NZ_breath_curve)
	{
		case 0x01:
			p_current_curve = a_curve;
			break;
		case 0x02:
			p_current_curve = b_curve;
			break;
		case 0x03:
			p_current_curve = c_curve;
			break;
		case 0x04:
			p_current_curve = d_curve;
			break;
		case 0x05:
			p_current_curve = e_curve;
			break;
		case 0x06:
			p_current_curve = f_curve;
			break;
	}
	mvprintw(1,0,"S=%02d", NZ_breath_curve);
	move (1,3);
	NZ_refresh();
}

void NZ_refresh_transpose()
{
	float f;
	if ( NZ_pitch_detune == 0) mvprintw(1,5,"T=000.0");
	else
	{
		f= (float)NZ_pitch_detune /10;
		if ( f < 0.0 ) mvprintw(1,5,"T=-%04.1f", fabs(f));
		else mvprintw(1,5,"T=+%04.1f", f);
	}
	move(1,9);
	NZ_refresh();
}

void NZ_refresh_main_menu(void)
{
	clear();
	NZ_refresh_font_name();
	NZ_refresh_volume();
	NZ_refresh();
	NZ_refresh_breath_curve();
	NZ_refresh_transpose();
	NZ_refresh_midi_mode();
#if defined (__SPDIF_ADAPTER__)
	NZ_refresh_audio_device();
#else
	mvprintw(1,20,"Info");
#endif
	NZ_refresh();
}



void NZ_set_breath_curve(void)
{
	move(1,3);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if (NZ_breath_curve > 1 ) 
					NZ_breath_curve--;
				break;
			case '+':
				if (NZ_breath_curve < MAX_BREATH_MODE ) 
					NZ_breath_curve++;
				break;
			case '1': 
				curs_set(0);
				NZ_refresh_breath_curve();
				NZ_refresh();
				return;
				break;
			default:
				NZ_control_volume(key);
				break;
		}
		NZ_refresh_breath_curve();
	}
}

void NZ_set_transpose(void)
{
	move(1,9);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if ( NZ_pitch_detune > -120 )
				{
					NZ_pitch_detune--;
					NZ_synth_detune(NZ_pitch_detune);
				}
				break;
			case '+':
				if ( NZ_pitch_detune < 120 )
				{
					NZ_pitch_detune++;
					NZ_synth_detune(NZ_pitch_detune);
				}
				break;
			case '2': 
				curs_set(0);
				return;
				break;
			default:
				NZ_control_volume(key);
				break;
		}
		NZ_refresh_transpose();
	}
}

void NZ_set_midi_mode(void)
{
	move(1,17);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if ( NZ_midi_mode > 1 ) NZ_midi_mode--;
				break;
			case '+':
				if ( NZ_midi_mode < MAX_MIDI_MODE ) NZ_midi_mode++;
				break;
			case '3': 
				NZ_refresh_midi_mode();
				curs_set(0);
				NZ_refresh();
				return;
				break;
			default:
				NZ_control_volume(key);
				break;
		}
		NZ_refresh_midi_mode();
		move(1,17);
		NZ_refresh();
	}
}

void NZ_control_volume(int k)
{
	switch (k)
	{
		/* Volume general */
		case 'A':
			NZ_increment_pcm_volume();
			break;
		case 'B':
			NZ_decrement_pcm_volume();
			break;
		/* Balance */
		case 'D':
			NZ_increment_right_pcm_volume();
			break;
		case 'C':
			NZ_increment_left_pcm_volume();
			break;
	}
	NZ_refresh_volume();
}

#if defined (__SPDIF_ADAPTER__)
void NZ_set_audio_device(void)
{
	move(1,22);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if (NZ_audio_device == 0x02 ) 
				{
					NZ_audio_device--;
					fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp1");
				}
				break;
			case '+':
				if (NZ_audio_device == 0x01 ) 
				{
					NZ_audio_device++;
					fluid_settings_setstr(synth_settings, "audio.oss.device", "/dev/dsp2");
				}
				break;
			case '4': 
				curs_set(0);
				NZ_refresh_audio_device();
				NZ_refresh();
				return;
				break;
			default:
				NZ_control_volume(key);
				break;
		}
		NZ_refresh_audio_device();
	}
}

#else
void NZ_info_menu(void)
{
	/*
         * Pas d'attente du clavier pour permettre le raffraichissement
	 */
	nodelay(screen,TRUE);
	double load;
	float temp;
	uint64_t mem;
	extern fluid_synth_t* synth;

	clear();
	mvprintw(1,20,"Exit");
	while (1)
	{
		temp=NZ_get_cpu_temperature();
		mem=NZ_get_free_memory();
		load=fluid_synth_get_cpu_load(synth);
		mvprintw(0,0,"Idle=%02.2f%%   Temp=%02.1fC", 100 - load, temp);
		mvprintw(1,0,"Free=%dK",mem); 
		NZ_refresh();
		usleep (400000);
		
		key=getch();
		switch (key) 
		{
			case '1': case '2': case '3': case '4':
			nodelay(screen,FALSE);
			return;
		}
	}
}
#endif


int *NZ_main_menu (void)
{
	/* Init curses */
	screen=initscr();
	noecho();
	keypad(stdscr, TRUE);
	raw();
	cbreak();
	curs_set(0);
	clear();
	NZ_refresh();

	/* Splash screen */

	mvprintw(0,0,"     Noizebox %s     \n  (c)%s L Hondareyte  ", VERSION, YEAR);
	NZ_refresh();
	sleep(1);

	/*
	 * Menu principal
	 */
	NZ_refresh_main_menu();

	/* 
	 * Boucle d'attente utilisateur 
	 */
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '1':
				NZ_set_breath_curve();
				break;
			case '2':
				NZ_set_transpose();
				break;
			case '3':
				NZ_set_midi_mode();
				break;
			case '4':
#if defined (__SPDIF_ADAPTER__)
				NZ_set_audio_device();
#else
				NZ_info_menu();
#endif
				NZ_refresh_main_menu();
				break;
			case '-':
				mvprintw(0,2,"Loading      ");NZ_refresh();
				NZ_load_font(current_font--);
				NZ_refresh_font_name();
				break;
			case '+':
				mvprintw(0,2,"Loading      ");NZ_refresh();
				NZ_load_font(current_font++);
				NZ_refresh_font_name();
				break;
#ifdef __NOIZEBOX_DEBUG__
			case 'V':
				NZ_maximize_pcm_volume();
				break;
			case 'v':
				NZ_mute_pcm_volume();
				break;
#endif
			case 'q':
				NZ_terminate_menu();
				return(0);
				break;
			default:
				NZ_control_volume(key);
				break;
		}
	}
	return 0;
}
