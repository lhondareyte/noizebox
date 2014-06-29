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

static WINDOW *screen;
int key;

void NZ_refresh(void)
{
	/* Rustine pour fonctionnement avec tinyVT */
	usleep (25000);
	refresh();
}

void noizebox_terminate_menu(void)
{
	clear();
        mvprintw(0,0,"  Shutdown in progress\n      Please wait...  ");
        NZ_refresh();
        endwin();
}

void NZ_refresh_volume(void)
{
	int16_t v;
	v=noizebox_get_pcm_volume();
	if ( prev_v != v ) 
	{
		mvprintw(0,15,"V=%03d:%03d", (v & 0xFF), (v >> 8));
		NZ_refresh();
	}
}

void NZ_refresh_midi_mode(void)
{
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

void NZ_refresh_font_name(void)
{
	mvprintw(0,0,"P=%-10s", current_font_name);
	NZ_refresh();
}

void NZ_refresh_sensitivity()
{
	mvprintw(1,0,"S=%02d", noizebox_noteon_minimum);
	move(1,3);
	NZ_refresh();
}

void NZ_refresh_transpose()
{
	float f;
	if ( NZ_current_pitch_detune == 0) mvprintw(1,6,"T=000.0");
	else
	{
		f= (float)NZ_current_pitch_detune /10;
		if ( f < 0.0 ) mvprintw(1,6,"T=-%04.1f", fabs(f));
		else mvprintw(1,6,"T=+%04.1f", f);
	}
	move(1,10);
	NZ_refresh();
}

void NZ_refresh_main_menu(void)
{
	clear();
	NZ_refresh_font_name();
	NZ_refresh_volume();
	NZ_refresh_sensitivity();
	NZ_refresh_transpose();
	NZ_refresh_midi_mode();
	mvprintw(1,20,"Info");
	NZ_refresh();
}



void NZ_set_sensitivity(void)
{
	move(1,3);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if (noizebox_noteon_minimum >= 1 ) 
					noizebox_noteon_minimum--;
				break;
			case '+':
				if (noizebox_noteon_minimum <= 98 ) 
					noizebox_noteon_minimum++;
				break;
			case '1': 
				curs_set(0);
				return;
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
		NZ_refresh_sensitivity();
	}
}

void NZ_set_transpose(void)
{
	move(1,10);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				if ( NZ_current_pitch_detune > -120 )
				noizebox_synth_detune(-1);
				break;
			case '+':
				if ( NZ_current_pitch_detune < 120 )
				noizebox_synth_detune(1);
				break;
			case '2': 
				curs_set(0);
				return;
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
		NZ_refresh_transpose();
	}
}

void NZ_set_midi_mode(void)
{
	move(1,16);
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
				if ( NZ_midi_mode < NZ_MAX_MIDI_MODE ) NZ_midi_mode++;
				break;
			case '3': 
				NZ_refresh_midi_mode();
				curs_set(0);
				NZ_refresh();
				return;
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
		NZ_refresh_midi_mode();
		move(1,17);
		NZ_refresh();
	}
}

void noizebox_control_volume(int k)
{
	switch (k)
	{
		/* Volume general */
		case 'A':
			noizebox_increment_pcm_volume();
			break;
		case 'B':
			noizebox_decrement_pcm_volume();
			break;
		/* Balance */
		case 'D':
			noizebox_increment_right_pcm_volume();
			break;
		case 'C':
			noizebox_increment_left_pcm_volume();
			break;
	}
	NZ_refresh_volume();
}

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

	while (1)
	{
		temp=noizebox_get_cpu_temperature();
		mem=noizebox_get_free_memory();
		load=fluid_synth_get_cpu_load(synth);
		mvprintw(0,0,"Idle=%02.2f%%   Temp=%02.1fC", 100 - load, temp);
		NZ_refresh();
		mvprintw(1,0,"Free=%dK",mem); clrtoeol();
		mvprintw(1,20,"Exit");
		NZ_refresh();
		usleep (450000);
		
		key=getch();
		switch (key) 
		{
			case '1': case '2': case '3': case '4':
			nodelay(screen,FALSE);
			return;
		}
	}
}


int *noizebox_main_menu (void)
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
	mvprintw(0,0,"     Noizebox v0.8a     \n  (c)2013 L Hondareyte");
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
				NZ_set_sensitivity();
				break;
			case '2':
				NZ_set_transpose();
				break;
			case '3':
				NZ_set_midi_mode();
				break;
			case '4':
				NZ_info_menu();
				NZ_refresh_main_menu();
				break;
			case '-':
				mvprintw(0,2,"Loading      ");NZ_refresh();
				noizebox_load_font(current_font--);
				NZ_refresh_font_name();
				break;
			case '+':
				mvprintw(0,2,"Loading      ");NZ_refresh();
				noizebox_load_font(current_font++);
				NZ_refresh_font_name();
				break;
#ifdef __NOIZEBOX_DEBUG__
			case 'V':
				noizebox_maximize_pcm_volume();
				break;
			case 'v':
				noizebox_mute_pcm_volume();
				break;
#endif
			case 'q':
				noizebox_terminate_menu();
				return(0);
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
	}
	return 0;
}
