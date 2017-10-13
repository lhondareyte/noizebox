/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#ifndef __NOIZEBOX_FUNCTIONS_H__
#define __NOIZEBOX_FUNCTIONS_H__

/*
 *  ihm.c
 */
int NZ_main_menu(void);
void NZ_close_mixer(void);
int  NZ_terminate_menu(int);
void NZ_set_breath_curve(void);
void NZ_set_transpose(void);
void NZ_set_midi_mode(void);

/*
 *  conf.c
 */
int NZ_load_synth_config(void);
int NZ_save_synth_config(void);

/*
 * bank.c
 */

void NZ_load_bank(void);
void NZ_load_font(int);

/*
 * synth.c
 */

void NZ_delete_synth(void);
void NZ_create_synth(void);
void NZ_load_next_font(void);
void NZ_load_prev_font(void);

void NZ_pitch_up(void);
void NZ_pitch_down(void);

void NZ_synth_detune(int);

/*
 * mixer.c
 */

int 	NZ_init_mixer(void);
int 	NZ_get_pcm_volume(void);
void 	NZ_mute_pcm_volume(void);
void 	NZ_maximize_pcm_volume(void);
void 	NZ_increment_pcm_volume(void);
void 	NZ_decrement_pcm_volume(void);
void 	NZ_increment_right_pcm_volume(void);
void 	NZ_increment_left_pcm_volume(void);
void 	NZ_decrement_line_volume(void);
void 	NZ_increment_line_volume(void);

/*
 * hinv.c
 */
float NZ_get_cpu_temperature(void);
uint64_t NZ_get_free_memory(void);

/*
 * ihm.c
 */
void NZ_control_volume(int);


#endif
