/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>

#include "bank.h"
#include "conf.h"
#include "ihm.h"
#include "mixer.h"
#include "noizebox.h"
#include "synth.h"

struct sigaction shutdown_action;

#if defined (__LEGACY_MIDI_PARSER__)
extern void *NZ_midi_read(void *);
#endif

int main(int argc, char *argv[])
{
	FILE *conf;
	if ( argc <= 1 ) {
		printf("Error: You must specify a OSS midi device.\n");
		goto error;
	}

	NZDIR=getenv("NZDIR");
	if ( ! NZDIR ) {
		printf ("Error; NZDIR is not set!\n");
		goto error;
	}
	sprintf(CONF_DB,"%s/Resources/noizebox.conf",NZDIR);
	conf = fopen (CONF_DB, "r" ) ;
	if ( conf == NULL )
		sprintf(CONF_DB,"/etc/noizebox.conf");
	else
		fclose(conf);

	signal(SIGINT, NZ_shutdown);
	signal(SIGTERM, NZ_shutdown);
	setpriority(PRIO_PROCESS, getpid(), PRIO_MAX);

	sprintf(FONT_DB,"%s/Resources/soundfont.conf",NZDIR);
	if (NZ_load_synth_config() == -1)
		goto error;

	if (NZ_create_synth() == -1) {
		goto error;
	}

        if (NZ_init_mixer() == -1) {
		goto error;
	}

        NZ_load_bank();
        NZ_load_font(startup_font);

#if defined (__LEGACY_MIDI_PARSER__)
	pthread_t threads[2];
	/* One thread per MIDI device */
	int c=1;
	while ( c < argc ) {
		if (pthread_create(&threads[c-1], NULL, NZ_midi_read, argv[c])) {
			perror("Error: Cannot create MIDI thread");
			goto error;
		}
		c++;
	}
#endif
	NZ_shutdown(NZ_main_menu());
error:
	exit (1);
}
