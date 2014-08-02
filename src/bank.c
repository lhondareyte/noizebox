/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include "noizebox.h"
#include "functions.h"
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
	int  i;
        char sql[80];
        sqlite3 *bank;
        sqlite3_stmt *stmt;

	/* Reset du tuning pour tous les canaux */
	for ( i=0; i<= 15; i++ )
	{
		fluid_synth_reset_tuning(synth, i);
	}

	if ( current_font_id != 0 ) 
	{
		fluid_synth_sfunload(synth,current_font_id,1);
	}
	if (current_font == 0 ) current_font=max_font_in_bank;
	if (current_font == max_font_in_bank + 1) current_font=1;
        
        sqlite3_initialize();
        if ( sqlite3_open_v2(FONT_DB, &bank, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK )
        {
                fprintf (stderr, "Error: cannot open SF2 configuration file\n");
                exit (1);
        }
	
	sprintf (sql, "select name, file from bank where id='%d'", current_font);
        if ( sqlite3_prepare_v2(bank,sql,strlen(sql),&stmt,NULL) == SQLITE_OK )
        {
		if ( sqlite3_step(stmt) == SQLITE_ROW ) 
		{
			sprintf(current_font_name,"%s",sqlite3_column_text(stmt,0));
			sprintf(current_font_path,"%s/Resources/SF2/%s",NZDIR,sqlite3_column_text(stmt,1));
			sprintf(ramdisk_font_path,"/SF2/%s",sqlite3_column_text(stmt,1));
                	sqlite3_finalize(stmt);
		}
		
        }
        else
        {
                fprintf (stderr, "Failed to prepare database: %s\n",sqlite3_errmsg(bank));
        }
//
        sqlite3_close(bank);
        sqlite3_shutdown();

	fluid_synth_system_reset(synth);

	current_font_id=fluid_synth_sfload(synth,ramdisk_font_path,1);
	if ( current_font_id == FLUID_FAILED ) 
	{
		current_font_id=fluid_synth_sfload(synth,current_font_path,1);
	}
	fprintf (stderr,"Loading SF2 %s (%d)\n",current_font_path, current_font_id);

	/* Actication du tuning pour tous les canaux */
	for ( i=0; i<= 15; i++)
	{
		fluid_synth_select_tuning(synth, i, 1, 1);
	}
	/* Retablissement du tuning courant */
	noizebox_synth_detune(NZ_pitch_detune);
}
