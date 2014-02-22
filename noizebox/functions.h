/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#ifndef __NOIZEBOX_FUNCTIONS_H__
#define __NOIZEBOX_FUNCTIONS_H__

/*
 *  ihm.c
 */
int *noizebox_main_menu(void);
void noizebox_terminate_menu(void);
void noizebox_set_sensitivity(void);
void noizebox_set_transpose(void);
void noizebox_set_midi_mode(void);

/*
 *  conf.c
 */
int noizebox_load_synth_config(void);
int noizebox_save_synth_config(void);

/*
 * bank.c
 */

void noizebox_load_bank(void);
int noizebox_load_font(int);

/*
 * synth.c
 */

void noizebox_delete_synth(void);
void noizebox_create_synth(void);
void noizebox_load_next_font(void);
void noizebox_load_prev_font(void);

void noizebox_pitch_up(void);
void noizebox_pitch_down(void);

/*
 * mixer.c
 */

int 	noizebox_init_mixer(void);
int 	noizebox_get_pcm_volume(void);
void 	noizebox_mute_pcm_volume(void);
void 	noizebox_maximize_pcm_volume(void);
void 	noizebox_increment_pcm_volume(void);
void 	noizebox_decrement_pcm_volume(void);
void 	noizebox_increment_right_pcm_volume(void);
void 	noizebox_increment_left_pcm_volume(void);
void 	noizebox_decrement_line_volume(void);
void 	noizebox_increment_line_volume(void);

/*
 * hinv.c
 */
float noizebox_get_cpu_temperature(void);
uint64_t noizebox_get_free_memory(void);

/*
 * ihm.c
 */
void noizebox_control_volume(int);


#endif
