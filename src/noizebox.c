/*
 * Copyright (c)2013-2017, Luc Hondareyte
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include "noizebox.h"
#include "functions.h"

extern void NZ_create_synth(void);
extern void NZ_delete_synth(void);

struct sigaction shutdown_action;

#if defined (__LEGACY_MIDI_PARSER__)
extern void *NZ_midi_read();
#endif

int main(int argc, char *argv[]) {
	FILE *conf;
	if ( argc <= 1 ) {
		printf("Error: You must specify a OSS midi device.\n");
		exit (1);
	}

#if defined (__LEGACY_MIDI_PARSER__)
	pthread_t threads[2];
	/* One thread per MIDI device */
	int c=1;
	while ( c < argc ) {
		if (pthread_create(&threads[c-1], NULL, NZ_midi_read, argv[c])) {
			perror("Error: Cannot create MIDI thread");
			exit (-1);
		}
		c++;
	}
#endif
	NZDIR=getenv("NZDIR");
	if ( ! NZDIR ) {
		printf ("Error; NZDIR is not set!\n");
		exit (1);
	}
	sprintf(CONF_DB,"%s/Resources/noizebox.conf",NZDIR);
	conf = fopen (CONF_DB, "r" ) ;
	if ( conf == NULL ) {
		sprintf(CONF_DB,"/etc/noizebox.conf");
	}
	else fclose(conf);

	sprintf(FONT_DB,"%s/Resources/soundfont.conf",NZDIR);
	signal(SIGINT, NZ_shutdown);
	signal(SIGTERM, NZ_shutdown);

	setpriority(PRIO_PROCESS, getpid(), PRIO_MAX);
	if ( NZ_load_synth_config() == -1) {
		exit (1);
	}

	NZ_create_synth();
	NZ_shutdown(NZ_main_menu());
}
