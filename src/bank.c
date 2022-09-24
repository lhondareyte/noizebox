/*
 * Copyright (c)2013-2021, Luc Hondareyte
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

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

#include "conf.h"
#include "global.h"
#include "synth.h"

int max_font;
extern char FONT_DB[256];

void NZ_load_bank(void)
{
	char * sql;
	sqlite3 *bank;
	sqlite3_stmt *stmt;

	sqlite3_initialize();
	if ( sqlite3_open_v2(FONT_DB, &bank, \
				SQLITE_OPEN_READONLY, NULL) != SQLITE_OK ) {
		fprintf (stderr, "Error: Cannot open SF2 configuration file (%s)\n",FONT_DB);
		exit (1);
	}
	sql = "select count(rowid) from bank" ;
	if ( sqlite3_prepare_v2(bank,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) {
		if (  sqlite3_step(stmt) == SQLITE_ROW ) {
			max_font=sqlite3_column_int(stmt,0);
			sqlite3_finalize(stmt);
		}
	}
	else {
		fprintf (stderr, "Error: Failed to prepare database: %s\n",sqlite3_errmsg(bank));
		exit (1);
	}
	sqlite3_close(bank);
	sqlite3_shutdown();
}

int NZ_load_font(int font)
{
	int  i;
	char sql[80];
	sqlite3 *bank;
	sqlite3_stmt *stmt;

	char sf2_ramdisk[256];
	char sf2_library[256];

	NZ_load_parameter(CONF_DB, "paths", "sf2_ramdisk_path", sf2_ramdisk );
	NZ_load_parameter(CONF_DB, "paths", "sf2_library_path", sf2_library );

	struct stat st;

	/* Reset tuning for all channels */
	for ( i=0; i<= 15; i++ ) {
		fluid_synth_deactivate_tuning(synth, i, FALSE);
	}

	if ( fluid_synth_sfcount (synth) > 0 ) {
		fprintf (stderr,"Unloading SF2 (id %d)\n", current_font);
		fluid_synth_sfunload(synth,current_font,1);
	}

	if (( font > max_font ) || (font < 0)) {
		font = 1;
	}
	sqlite3_initialize();
	if ( sqlite3_open_v2(FONT_DB, &bank, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK ) {
		fprintf (stderr, "Error: Cannot open SF2 configuration file\n");
		exit (1);
	}

	sprintf (sql, "select name, file, key_offset from bank where rowid='%d'", font);
	if ( sqlite3_prepare_v2(bank,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) {
		if ( sqlite3_step(stmt) == SQLITE_ROW ) {
			sprintf(current_font_name,"%s",sqlite3_column_text(stmt,0));
			sprintf(current_font_path,"%s/%s",sf2_library,sqlite3_column_text(stmt,1));
			sprintf(ramdisk_font_path,"%s/%s",sf2_ramdisk,sqlite3_column_text(stmt,1));
			font_key_offset=sqlite3_column_int(stmt,2);
			sqlite3_finalize(stmt);
		}
		else {
			fprintf (stderr, "Error: Failed to get soundfont path for id : %d\n", font);
			exit (1);
		}

	}
	else {
		fprintf (stderr, "Error: Failed to prepare database: %s\n",sqlite3_errmsg(bank));
		exit (1);
	}

	sqlite3_close(bank);
	sqlite3_shutdown();

	fluid_synth_system_reset(synth);

	if (stat(ramdisk_font_path, &st) == 0) {
#ifdef __NOIZEBOX_DEBUG__
		fprintf (stderr,"Loading SF2 %s (%d)\n",ramdisk_font_path, font);
#endif
		current_font=fluid_synth_sfload(synth,ramdisk_font_path,1);
	}
	else {
#ifdef __NOIZEBOX_DEBUG__
		fprintf (stderr,"Loading SF2 %s (%d)\n",current_font_path, font);
#endif
		current_font=fluid_synth_sfload(synth,current_font_path,1);
	}

	/* turn on tuning for all channels */
	for ( i=0; i<= 15; i++) {
		fluid_synth_activate_tuning(synth, i, 1, 1, FALSE);
	}
	/* Restore current tuning */
	NZ_synth_detune(NZ_pitch_detune);
	return font;
}

void NZ_load_next_font(void) {
	startup_font++;
	if (startup_font > max_font) {
		startup_font = 1;
	}
	NZ_load_font(startup_font);
}

void NZ_load_prev_font(void) {
	startup_font--;
	if (startup_font < 1 ) {
		startup_font = max_font;
	}
	NZ_load_font(startup_font);
}

