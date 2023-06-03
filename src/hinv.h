/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2021-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#ifndef __NOIZEBOX_HINV_H__
#define __NOIZEBOX_HINV_H__

int NZ_get_vcpus(void);
float NZ_get_cpu_temperature(void);
uint64_t NZ_get_free_memory(void);

#endif // NOIZEBOX_HINV_H__
