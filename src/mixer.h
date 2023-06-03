/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2021, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_MIXER_H__
#define __NOIZEBOX_MIXER_H__

int  NZ_init_mixer(void);
int  NZ_get_pcm_volume(void);
void NZ_close_mixer(void);
void NZ_mute_pcm_volume(void);
void NZ_maximize_pcm_volume(void);
void NZ_increment_pcm_volume(void);
void NZ_decrement_pcm_volume(void);
void NZ_increment_right_pcm_volume(void);
void NZ_increment_left_pcm_volume(void);
void NZ_decrement_line_volume(void);
void NZ_increment_line_volume(void);

#endif // NOIZEBOX2_MIXER_H
