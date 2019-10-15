/*
 * Copyright (c)2017, Luc Hondareyte
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

#ifndef __NOIZEBOX2_FUNCTIONS_H__
#define __NOIZEBOX2_FUNCTIONS_H__

/*
 *  ihm.c
 */
int  NZ_main_menu(void);
void NZ_close_mixer(void);
void NZ_shutdown(int);
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
void NZ_synth_detune(int);

/*
 * mixer.c
 */

int  NZ_init_mixer(void);
int  NZ_get_pcm_volume(void);
void NZ_mute_pcm_volume(void);
void NZ_maximize_pcm_volume(void);
void NZ_increment_pcm_volume(void);
void NZ_decrement_pcm_volume(void);
void NZ_increment_right_pcm_volume(void);
void NZ_increment_left_pcm_volume(void);
void NZ_decrement_line_volume(void);
void NZ_increment_line_volume(void);

/*
 * hinv.c
 */

float NZ_get_cpu_temperature(void);
uint64_t NZ_get_free_memory(void);

/*
 * ihm.c
 */

void NZ_control_volume(int);

#endif // NOIZEBOX2_FUNCTIONS_H__
