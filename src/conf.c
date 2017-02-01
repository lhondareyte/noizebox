/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <sqlite3.h>
#include "noizebox.h"
#include "functions.h"

int NZ_load_synth_parameter(char * col, char * val)
{
	int rc=0;
	char sql[80];
	sqlite3 *db;
	sqlite3_stmt *stmt;
	if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK )
	{
		fprintf (stderr, "Error: Cannot open configuration file (%s)\n", CONF_DB);
		return -1;
	}
	sprintf (sql, "select val from %s where param=\'%s\'", col, val);
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
                if (  sqlite3_step(stmt) == SQLITE_ROW )
                {
                        rc=sqlite3_column_int(stmt,0);
                        sqlite3_finalize(stmt);
                }
                else sqlite3_reset(stmt);
        }
        else
        {
                fprintf (stderr, "Error: Failed to prepare database: %s\n",sqlite3_errmsg(db));
                return -1;
        }
	sqlite3_close(db);
	return rc; 
}

int NZ_load_synth_config(void)
{
	int rc = 0;

#ifdef __SPDIF_ADAPTER__
	rc=NZ_load_synth_parameter("dsp","device");
	if ( rc != -1 ) NZ_audio_device=rc;
	else NZ_audio_device=DEFAULT_PCM_DEVICE;
#endif

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

int NZ_save_synth_parameter(char * tab, char * col, int val)
{

	int rc;
        char sql[80];
        sqlite3 *db;
        sqlite3_stmt *stmt;

        if ( sqlite3_open(CONF_DB, &db) != SQLITE_OK )
        {
                fprintf (stderr, "Error: Cannot open configuration file\n");
                return -1;
        }
        sprintf (sql, "update %s set val=%d where param=\'%s\'", tab, val, col);
        if ( sqlite3_prepare_v2(db,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
        }
        else
        {
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

int NZ_save_synth_config(void)
{
	int v,r,l;


#if defined (__SPDIF_ADAPTER__)
	/*
	 * Update Audio Device
   	 */
	if (NZ_save_synth_parameter("dsp", "device", NZ_audio_device) == -1)
	{
		fprintf (stderr, "Error: Cannot update device information: %s\n", sqlite3_errmsg(db));
		return -1;
	}
#endif
	
	/*
	 * Update Volume
   	 */
	v=NZ_get_pcm_volume();
	l = v & 0xff;
	r = v >> 8;
	if (NZ_save_synth_parameter("mixer", "left", l) == -1)
	{
		fprintf (stderr, "Error: Cannot save volume (l) information!\n"); 
		return -1;
	}
	
	if (NZ_save_synth_parameter("mixer", "right", r) == -1)
	{
		fprintf (stderr, "Error: Cannot save volume (r) information!\n");
		return -1;
	}
	
	/*
	 * Update SoundFont information
   	 */
	if (NZ_save_synth_parameter("soundfont", "last_id", current_font) == -1)
	{
		fprintf (stderr, "Error: Cannot save soundfont information!\n");
		return -1;
	}
	
	/*
	 * Update MIDI information
   	 */
	if (NZ_save_synth_parameter("midi", "mode", NZ_midi_mode) == -1)
	{
		fprintf (stderr, "Error: Cannot save MIDI mode information!\n");
		return -1;
	}
	
	if (NZ_save_synth_parameter("midi", "breath_curve", NZ_breath_curve) == -1)
	{
		fprintf (stderr, "Error: Cannot save breath mode information!\n");
		return -1;
	}
	
	/*
	 * Update SYNTH information
   	 */
	if (NZ_save_synth_parameter("synth", "detune", NZ_pitch_detune) == -1)
	{
		fprintf (stderr, "Error: Cannot save detune information!\n");
		return -1;
	}
	
	return 0;
}
