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

extern void NZ_create_synth(void);
extern void NZ_delete_synth(void);

struct sigaction shutdown_action;

#ifndef __FLUIDSYNTH_MIDI_DRIVER__

extern void *NZ_midi_read();

void  NZ_shutdown(int signum)
{
	int rc;
	NZ_terminate_menu();
	rc=NZ_save_synth_config();
	NZ_delete_synth();
#ifndef __FLUIDSYNTH_MIDI_DRIVER__
	pthread_kill(0,9);
#endif
	NZ_close_mixer();
	exit (rc);

}

int main(int argc, char *argv[])
{
	FILE *conf;
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

	shutdown_action.sa_handler = &NZ_shutdown;
	shutdown_action.sa_flags = 0;
	sigemptyset(&shutdown_action.sa_mask);
	sigaction(SIGTERM, &shutdown_action,(struct sigaction *)0);
	//sigaction(SIGQUIT, &shutdown_action,(struct sigaction *)0);

#else

int main(void)
{
#endif

	pthread_t threads[2];

	setpriority(PRIO_PROCESS, getpid(), PRIO_MAX);
	if ( NZ_load_synth_config() == -1) 
	{
		exit (1);
	}

	prev_v=NZ_get_pcm_volume();

#ifndef __FLUIDSYNTH_MIDI_DRIVER__

	/* Creation d'un thread par device MIDI passe en argument */
	int c=1;
	while ( c < argc ) 
	{
		if (pthread_create(&threads[c-1], NULL, NZ_midi_read, argv[c]))
		{
			perror("Error: Cannot create MIDI thread");
			exit (-1);
		}
		c++;
	}
#endif
	NZ_create_synth();
	NZ_main_menu();
	NZ_shutdown(0);
	exit (0);
}
