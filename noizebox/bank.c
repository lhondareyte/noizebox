/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include "noizebox.h"

#ifndef __WITH_SQLITE__
void noizebox_load_next_font(void)
{
	if ( current_font_id != 0 ) fluid_synth_sfunload(synth,current_font_id,1);
	int f=current_font;
	f++;
	if ( noizebox_bank[f].name ==  NULL ) f=0;
	current_font_id=fluid_synth_sfload(synth,noizebox_bank[f].file,1);
	current_font=f;
}

void noizebox_load_prev_font(void)
{
	if ( current_font_id != 0 ) fluid_synth_sfunload(synth,current_font_id,1);
	int f=current_font;
	if (f == 0) f=max_font_in_bank;
	else f--;
	current_font_id=fluid_synth_sfload(synth,noizebox_bank[f].file,1);
	current_font=f;
}

#else

#include <sqlite3.h>


void noizebox_load_bank(void)
{
	char * sql;
	sqlite3 *bank;
	sqlite3_stmt *stmt;

	sqlite3_initialize();
        if ( sqlite3_open_v2(FONT_DB, &bank, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK )
        {
                fprintf (stderr, "Error: cannot open SF2 configuration file (%s)\n",FONT_DB);
                exit (1);
        }
	sql = "select count(id) from bank" ;
	if ( sqlite3_prepare_v2(bank,sql,strlen(sql),&stmt,NULL) == SQLITE_OK ) 
	{
		if (  sqlite3_step(stmt) == SQLITE_ROW ) 
		{
			max_font_in_bank=sqlite3_column_int(stmt,0);
			sqlite3_finalize(stmt);
		}
	}
	else
	{
		fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(bank));
	}
	sqlite3_close(bank);
	sqlite3_shutdown();
}

void noizebox_load_font(int font)
{
	int previous_offset=noizebox_font_pitch_offset;
        char sql[80];
        sqlite3 *bank;
        sqlite3_stmt *stmt;
	if ( current_font_id != 0 ) fluid_synth_sfunload(synth,current_font_id,1);
	if (current_font == 0 ) current_font=max_font_in_bank;
	if (current_font == max_font_in_bank + 1) current_font=1;
        
        sqlite3_initialize();
        if ( sqlite3_open_v2(FONT_DB, &bank, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK )
        {
                fprintf (stderr, "Error: cannot open SF2 configuration file\n");
                exit (1);
        }
	
	sprintf (sql, "select name, file, offset from bank where id='%d'", current_font);
        if ( sqlite3_prepare_v2(bank,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
		if ( sqlite3_step(stmt) == SQLITE_ROW ) 
		{
			sprintf(current_font_name,"%s",sqlite3_column_text(stmt,0));
			sprintf(current_font_path,"%s/Resources/SF2/%s",NZDIR,sqlite3_column_text(stmt,1));
			noizebox_font_pitch_offset=sqlite3_column_int(stmt,2);
                	sqlite3_finalize(stmt);
		}
        }
        else
        {
                fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(bank));
        }
        sqlite3_close(bank);
        sqlite3_shutdown();

	if ( previous_offset != noizebox_font_pitch_offset ) fluid_synth_system_reset(synth);

	fprintf (stderr,"Loading SF2 %s\n",current_font_path);
	current_font_id=fluid_synth_sfload(synth,current_font_path,1);
}

#endif
