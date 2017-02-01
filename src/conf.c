/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <sqlite3.h>
#include "noizebox.h"
#include "functions.h"

int NZ_load_synth_config(void)
{
	char * sql;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK )
	{
		fprintf (stderr, "Error: cannot open configuration file (%s)\n", CONF_DB);
		return 1;
	}

#if defined (__SPDIF_ADAPTER__)
	sql = "select val from dsp where param='device'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW ) 
		{ 
			NZ_audio_device=sqlite3_column_int(stmt,0);
			sqlite3_finalize(stmt);
		}
		else sqlite3_reset(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));
#endif

	sql = "select val from mixer where param='left'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW ) 
		{ 
			NZ_pcm_volume_left=sqlite3_column_int(stmt,0);
			sqlite3_finalize(stmt);
		}
		else sqlite3_reset(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));

	sql = "select val from mixer where param='right'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW )
		{ 
			NZ_pcm_volume_right=sqlite3_column_int(stmt,0);
			sqlite3_finalize(stmt);
		}
		else sqlite3_reset(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));

	sql = "select val from soundfont where param='last_id'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW )
		{
			current_font=sqlite3_column_int(stmt,0);
		}
		else 
		{
			current_font=1;
			sqlite3_reset(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));

	sql = "select val from midi where param='mode'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW )
		{
			NZ_midi_mode=sqlite3_column_int(stmt,0);
		}
		else 
		{
			NZ_midi_mode=1;
			sqlite3_reset(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));

        sql = "select val from midi where param='breath_curve'";
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
                if (  sqlite3_step(stmt) == SQLITE_ROW )
                {
                        NZ_breath_curve=sqlite3_column_int(stmt,0);
                }
                else
                {
                        NZ_breath_curve=1;
                        sqlite3_reset(stmt);
                }
                sqlite3_finalize(stmt);
        }
        else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));

	sql = "select val from synth where param='detune'";
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW )
		{
			NZ_pitch_detune=sqlite3_column_int(stmt,0);
		}
		else 
		{
			NZ_pitch_detune=0;
			sqlite3_reset(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(db));


	sqlite3_close(db);

	return 0;
}

int NZ_save_synth_config(void)
{
	int rc;
        char sql[80];
        sqlite3 *db;
        sqlite3_stmt *stmt;

	int v,r,l;

        if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK )
        {
                fprintf (stderr, "Error: cannot open configuration file\n");
                exit (1);
        }

#if defined (__SPDIF_ADAPTER__)
	/*
	 * Update Audio Device
   	 */
	sprintf (sql, "update dsp set val=%d where param=\'device\'",NZ_audio_device);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
#endif
	
	/*
	 * Update Volume
   	 */
	v=NZ_get_pcm_volume();
	l = v & 0xff;
	r = v >> 8;
	sprintf (sql, "update mixer set val=%d where param=\'left\'",l);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
	
	sprintf (sql, "update mixer set val=%d where param=\'right\'",r);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);
	
	/*
	 * Update SoundFont information
   	 */
	sprintf (sql, "update soundfont set val=%d where param=\'last_id\'",current_font);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);

	/*
	 * Update MIDI information
   	 */
	sprintf (sql, "update midi set val=%d where param=\'mode\'",NZ_midi_mode);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);

        sprintf (sql, "update midi set val=%d where param=\'breath_curve\'",NZ_breath_curve);
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
                sqlite3_step(stmt);
        }
        else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        
	/*
	 * Update SYNTH information
   	 */
	sprintf (sql, "update synth set val=%d where param=\'detune\'",NZ_pitch_detune);
	if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		sqlite3_step(stmt);
	}
	else fprintf (stderr, "Error: cannot update synth information: %s\n", sqlite3_errmsg(db));
	sqlite3_finalize(stmt);

	/*
	 * Update Synth parameters
   	 */
	sqlite3_finalize(stmt);
	rc=sqlite3_close(db);

	if ( rc != SQLITE_OK )
		fprintf (stderr, 
		"Error: cannot save configuration file: %s\n", 
		sqlite3_errmsg(db));
	return rc;
}
