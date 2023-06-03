/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2021-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_CONF_H__
#define __NOIZEBOX_CONF_H__

int NZ_load_synth_config(void);
int NZ_save_synth_config(void);
int NZ_load_parameter(char *, char *, char *, char *);
int NZ_save_parameter(char *, char *, char *, int);

#endif // NOIZEBOX_CONF_H__
