/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2017-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_IHM_H__
#define __NOIZEBOX_IHM_H__

int  NZ_main_menu(void);
void NZ_shutdown(int);
void NZ_set_breath_curve(void);
void NZ_set_transpose(void);
void NZ_set_midi_mode(void);
void NZ_control_volume(int);

#endif // NOIZEBOX_IHM_H__
