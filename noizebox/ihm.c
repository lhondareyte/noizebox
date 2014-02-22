/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <ncurses.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <fluidsynth.h>

#include "noizebox.h"
#include "functions.h"

static WINDOW *screen;
int key;

void noizebox_terminate_menu(void)
{
	clear();
        mvprintw(0,0,"Shutdown in progress\n    Please wait...  ");
        refresh();
        endwin();
}

void noizebox_refresh_volume(void)
{
	int16_t v;
	v=noizebox_get_pcm_volume();
	mvprintw(0,17,"%03d:%03d", (v & 0xFF), (v >> 8));
	refresh();
}

void noizebox_refresh_midi_mode(void)
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
	mvprintw(1,15,"%3s",current_midi_mode_name);
	refresh();
}

void noizebox_refresh_main_menu(void)
{
	mvprintw(0,0,"Prg=%8s Vol=---:---", current_font_name);
	mvprintw(1,0,"S=%02d  T=-0-  M=---  Info",
 		noizebox_noteon_minimum);
	refresh();
	noizebox_refresh_midi_mode();
	noizebox_refresh_volume();
}



void noizebox_set_sensitivity(void)
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
		mvprintw(1,2,"%02d",noizebox_noteon_minimum);
		move(1,3);
		refresh();
	}
}

void noizebox_set_transpose(void)
{
	move(1,9);
	curs_set(1);
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '-':
				noizebox_synth_detune(-100);
				break;
			case '+':
				noizebox_synth_detune(100);
				break;
			case '2': 
				curs_set(0);
				return;
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
		mvprintw(1,8,"%02d",noizebox_noteon_minimum);
		move(1,9);
		refresh();
	}
}

void noizebox_set_midi_mode(void)
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
				curs_set(0);
				return;
				break;
			default:
				noizebox_control_volume(key);
				break;
		}
		noizebox_refresh_midi_mode();
		move(1,16);
		refresh();
	}
}

void noizebox_control_volume(int k)
{
	switch (k)
	{
		/* Volume general */
		case KEY_UP:
			noizebox_increment_pcm_volume();
			break;
		case KEY_DOWN:
			noizebox_decrement_pcm_volume();
			break;
		/* Balance */
		case KEY_LEFT:
			noizebox_increment_right_pcm_volume();
			break;
		case KEY_RIGHT:
			noizebox_increment_left_pcm_volume();
			break;
	}
	noizebox_refresh_volume();
}

void noizebox_info_menu(void)
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
		mvprintw(1,0,"Free=%dK",mem); clrtoeol();
		mvprintw(1,20,"Exit");
		
		refresh();
		usleep(500000);
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
	curs_set(0);
	cbreak();
	clear();

	/* Splash screen */
	mvprintw(0,0,"   Noizebox V0.8    \n(c)2013 L Hondareyte");
	refresh();
	sleep(1);
	clear();
	refresh();

	/*
	 * Menu principal
	 */
	noizebox_refresh_main_menu();

	/* 
	 * Boucle d'attente utilisateur 
	 */
	while (1)
	{
		key=getch();
		switch (key)
		{
			case '1':
				noizebox_set_sensitivity();
				break;
			case '2':
				noizebox_set_transpose();
				break;
			case '3':
				noizebox_set_midi_mode();
				break;
			case '4':
				noizebox_info_menu();
				break;
			case '-':
				mvprintw(0,4,"Loading ");refresh();
				noizebox_load_font(current_font--);
				break;
			case '+':
				mvprintw(0,4,"Loading ");refresh();
				noizebox_load_font(current_font++);
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
		noizebox_refresh_main_menu();
	}
	return 0;
}
