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

#include <sqlite3.h>
#include <sys/types.h>
#include "functions.h"
#include "noizebox.h"
#include "mixer.h"

int NZ_load_synth_parameter(char * col, char * val) {
	int rc=0;
	char sql[80];
	sqlite3 *db;
	sqlite3_stmt *stmt;
	if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK ) {
		fprintf (stderr, "Error: Cannot open configuration file (%s)\n", CONF_DB);
		return -1;
	}
	sprintf (sql, "select val from %s where param=\'%s\'", col, val);
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) {
                if (  sqlite3_step(stmt) == SQLITE_ROW ) {
                        rc=sqlite3_column_int(stmt,0);
                        sqlite3_finalize(stmt);
                }
                else sqlite3_reset(stmt);
        }
        else {
                fprintf (stderr, "Error: Failed to prepare database: %s\n",sqlite3_errmsg(db));
                return -1;
        }
	sqlite3_close(db);
	return rc; 
}

int NZ_load_synth_config(void) {
	int rc = 0;

	rc=NZ_load_synth_parameter("mixer","left");
	if ( rc != -1 ) NZ_pcm_volume_left=rc;
	else NZ_pcm_volume_left=100;

	rc=NZ_load_synth_parameter("mixer","right");
	if ( rc != -1 ) NZ_pcm_volume_right=rc;
	else NZ_pcm_volume_right=100;

	rc=NZ_load_synth_parameter("soundfont","last_id");
	if ( rc > 0 ) current_font=rc;
	else current_font=1;

	rc=NZ_load_synth_parameter("midi","mode");
	if ( rc > 0 ) NZ_midi_mode=rc;
	else NZ_midi_mode=1;

	rc=NZ_load_synth_parameter("midi","breath_curve");
	if ( rc > 0 ) NZ_breath_curve=rc;
	else NZ_breath_curve=1;

	rc=NZ_load_synth_parameter("synth","detune");
	if ( rc != -1 ) NZ_pitch_detune=rc;
	else NZ_pitch_detune=0;

	return 0;
}

int NZ_save_synth_parameter(char * tab, char * col, int val) {

	int rc;
        char sql[80];
        sqlite3 *db;
        sqlite3_stmt *stmt;

        if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK ) {
                fprintf (stderr, "Error: Cannot open configuration file\n");
                return -1;
        }
        sprintf (sql, "update %s set val=%d where param=\'%s\'", tab, val, col);
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) {
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
        }
        else {
                fprintf (stderr, "Error: %s\n", sqlite3_errmsg(db));
                return -1;
        }

        rc=sqlite3_close(db);
        if ( rc != SQLITE_OK )
                fprintf (stderr,
                "Error: cannot save configuration file: %s\n",
                sqlite3_errmsg(db));
        return rc;
}

int NZ_save_synth_config(void) {

	int v,r,l;

	/*
	 * Update Volume
   	 */
	v=NZ_get_pcm_volume();
	l = v & 0xff;
	r = v >> 8;
	if (NZ_save_synth_parameter("mixer", "left", l) == -1) {
		fprintf (stderr, "Error: Cannot save volume (l) information!\n"); 
		return -1;
	}
	
	if (NZ_save_synth_parameter("mixer", "right", r) == -1) {
		fprintf (stderr, "Error: Cannot save volume (r) information!\n");
		return -1;
	}
	
	/*
	 * Update SoundFont information
   	 */
	if (NZ_save_synth_parameter("soundfont", "last_id", current_font) == -1) {
		fprintf (stderr, "Error: Cannot save soundfont information!\n");
		return -1;
	}
	
	/*
	 * Update MIDI information
   	 */
	if (NZ_save_synth_parameter("midi", "mode", NZ_midi_mode) == -1) {
		fprintf (stderr, "Error: Cannot save MIDI mode information!\n");
		return -1;
	}
	
	if (NZ_save_synth_parameter("midi", "breath_curve", NZ_breath_curve) == -1) {
		fprintf (stderr, "Error: Cannot save breath mode information!\n");
		return -1;
	}
	
	/*
	 * Update SYNTH information
   	 */
	if (NZ_save_synth_parameter("synth", "detune", NZ_pitch_detune) == -1) {
		fprintf (stderr, "Error: Cannot save detune information!\n");
		return -1;
	}
	
	return 0;
}
