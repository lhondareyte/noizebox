/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "noizebox.h"
#include "functions.h"

extern void noizebox_create_synth(void);
extern void noizebox_delete_synth(void);

struct sigaction shutdown_action;

#ifndef __FLUIDSYNTH_MIDI_DRIVER__

extern void *noizebox_midi_read();
pthread_t threads[2];

void  noizebox_shutdown(int signum)
{
	int rc;
	noizebox_terminate_menu();
	rc=noizebox_save_synth_config();
	noizebox_delete_synth();
#ifndef __FLUIDSYNTH_MIDI_DRIVER__
	pthread_kill(0,9);
#endif
	close (noizebox_mixer);
	exit (rc);

}

int main(int argc, char *argv[])
{
	FILE *conf;
	noizebox_noteon_minimum=0;
	NZDIR=getenv("NZDIR");
	if ( ! NZDIR ) 
	{
		printf ("Error; NZDIR is not set!\n");
		exit (1);
	}
	sprintf(CONF_DB,"%s/Resources/noizebox.conf",NZDIR);
	conf = fopen (CONF_DB, "r" ) ;
	if ( conf == NULL ) 
	{
		sprintf(CONF_DB,"/etc/noizebox.conf");
	}
	else fclose(conf);

	sprintf(FONT_DB,"%s/Resources/soundfont.conf",NZDIR);

	if ( argc <= 1 ) 
	{
		printf("Error: You must specify a OSS midi device.\n");
		exit (1);
	}

	shutdown_action.sa_handler = &noizebox_shutdown;
	shutdown_action.sa_flags = 0;
	sigemptyset(&shutdown_action.sa_mask);
	sigaction(SIGTERM, &shutdown_action,(struct sigaction *)0);
	//sigaction(SIGQUIT, &shutdown_action,(struct sigaction *)0);

#else

int main(void)
{
#endif

	setpriority(PRIO_PROCESS, getpid(), PRIO_MAX);
	if ( noizebox_load_synth_config() == -1) 
	{
		exit (1);
	}

	prev_v=noizebox_get_pcm_volume();

#ifndef __FLUIDSYNTH_MIDI_DRIVER__

	if (pthread_create(&threads[0], NULL, noizebox_midi_read, argv[1]))
	{
		perror("Error: Cannot create MIDI thread");
		exit (-1);
	}
	if ( argc == 3 )
	{
		if (pthread_create(&threads[1], NULL, noizebox_midi_read, argv[2]))
		{
			fprintf(stderr,"Error: Cannot create second MIDI thread");
			exit (-1);
		}
	}
#endif
	noizebox_create_synth();
	noizebox_main_menu();
	noizebox_shutdown(0);
	exit (0);
}
