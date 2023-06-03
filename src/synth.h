/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_SYNTH_H__
#define __NOIZEBOX_SYNTH_H__

#if defined ( __LEGACY_MIDI_PARSER__ )
  #include <pthread.h>
#endif

int  NZ_create_synth(void);
void NZ_delete_synth(void);
void NZ_synth_detune(int);

#endif /* __NOIZEBOX_SYNTH_H__ */
