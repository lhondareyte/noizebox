/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2022, Luc Hondareyte
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/types.h>

#include "global.h"
#include "mixer.h"

int NZ_load_parameter(char * database, char * table, char * param, char * str)
{
	int rc=0;
	char sql[256];
	sqlite3 *db;
	sqlite3_stmt *stmt;
	if ( sqlite3_open(database, &db) != SQLITE_OK ) {
		fprintf (stderr, "Error: Cannot open configuration file (%s)\n", database);
		return -1;
	}
	sprintf (sql, "select val from %s where param=\'%s\'", table, param);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) {
		if (  sqlite3_step(stmt) == SQLITE_ROW ) {
			if ( ! str ) {
				rc=sqlite3_column_int(stmt,0);
			}
			else {
				sprintf(str,"%s",sqlite3_column_text(stmt,0));
			}
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

int NZ_save_parameter(char * database, char * table, char * param, int val)
{
	int rc;
	char sql[256];
	sqlite3 *db;
	sqlite3_stmt *stmt;

	if ( sqlite3_open(database, &db) != SQLITE_OK ) {
		fprintf (stderr, "Error: Cannot open configuration file\n");
		return -1;
	}
	sprintf (sql, "update %s set val=%d where param=\'%s\'", table, val, param);
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
		fprintf (stderr, "Error: cannot save configuration file: %s\n",
				sqlite3_errmsg(db));
	return rc;
}

int NZ_load_synth_config(void)
{
	int rc = 0;

	rc=NZ_load_parameter(CONF_DB, "mixer", "left", NULL);
	if ( rc != -1 )
		NZ_pcm_volume_left=rc;
	else
		NZ_pcm_volume_left=100;

	rc=NZ_load_parameter(CONF_DB, "mixer", "right", NULL);
	if ( rc != -1 )
		NZ_pcm_volume_right=rc;
	else
		NZ_pcm_volume_right=100;

	rc=NZ_load_parameter(CONF_DB, "soundfont", "last_id", NULL);
	if ( rc > 0 )
		startup_font=rc;
	else
		startup_font=1;

	rc=NZ_load_parameter(CONF_DB, "midi", "mode", NULL);
	if ( rc > 0 )
		NZ_midi_mode=rc;
	else
		NZ_midi_mode=1;

	rc=NZ_load_parameter(CONF_DB, "midi", "breath_curve", NULL);
	if ( rc > 0 )
		NZ_breath_curve=rc;
	else
		NZ_breath_curve=1;

	rc=NZ_load_parameter(CONF_DB, "synth","detune", NULL);
	if ( rc != -1 )
		NZ_pitch_detune=rc;
	else
		NZ_pitch_detune=0;

	return 0;
}

int NZ_save_synth_config(void)
{
	int v,r,l;

	/* Update Volume */
	v=NZ_get_pcm_volume();
	l = v & 0xff;
	r = v >> 8;
	if (NZ_save_parameter(CONF_DB, "mixer", "left", l) == -1) {
		fprintf (stderr, "Error: Cannot save volume (l) configuration!\n"); 
		return -1;
	}

	if (NZ_save_parameter(CONF_DB, "mixer", "right", r) == -1) {
		fprintf (stderr, "Error: Cannot save volume (r) configuration!\n");
		return -1;
	}

	/* Update SoundFont configuration */
	if (NZ_save_parameter(CONF_DB, "soundfont", "last_id", startup_font) == -1) {
		fprintf (stderr, "Error: Cannot save soundfont configuration!\n");
		return -1;
	}

	/* Update MIDI configuration */
	if (NZ_save_parameter(CONF_DB, "midi", "mode", NZ_midi_mode) == -1) {
		fprintf (stderr, "Error: Cannot save MIDI mode configuration!\n");
		return -1;
	}

	if (NZ_save_parameter(CONF_DB, "midi", "breath_curve", NZ_breath_curve) == -1) {
		fprintf (stderr, "Error: Cannot save breath mode configuration!\n");
		return -1;
	}

	/* Update SYNTH configuration */
	if (NZ_save_parameter(CONF_DB, "synth", "detune", NZ_pitch_detune) == -1) {
		fprintf (stderr, "Error: Cannot save detune configuration!\n");
		return -1;
	}

	return 0;
}
