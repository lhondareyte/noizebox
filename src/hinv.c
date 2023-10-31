/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)2013-2022, Luc Hondareyte
 * All rights reserved.
 * 
 */

#include <sys/types.h>
#ifdef __FreeBSD__
#include <sys/sysctl.h>
#endif
#ifdef __linux__
#include <stdint.h>
#endif
#include <unistd.h>

#include "global.h"
#include "hinv.h"

int NZ_get_ncpus(void)
{
	int ncpus=1;
#ifdef __FreeBSD__
	size_t size = sizeof vcpus;
        sysctlbyname("hw.ncpu", &ncpus, &size, NULL, 0);
#endif
	return ncpus;
}

uint64_t NZ_get_free_memory(void)
{
#ifdef __FreeBSD__
        int pagesize=0;
        int inactive=0;
        int free=0;
        size_t size = sizeof free;

        sysctlbyname("vm.stats.vm.v_free_count", &free, &size, NULL, 0);
        sysctlbyname("vm.stats.vm.v_inactive_count", &inactive, &size, NULL, 0);
        size = sizeof pagesize;
        sysctlbyname("vm.stats.vm.v_page_size", &pagesize, &size, NULL, 0);
	return (free + inactive) * pagesize / 1024 ;
#endif
	return 0;
}

float NZ_get_cpu_temperature(void)
{
	float temp = 20.0;
#ifdef __FreeBSD__
        int buf;
        size_t size = sizeof buf;

        if (sysctlbyname("hw.acpi.thermal.tz0.temperature", &buf, &size, NULL, 0) == 0)
		/* deci-Kelvin -> Celsius */
		temp = ((float)buf-2732)/10;
	else if (sysctlbyname("dev.cpu.0.temperature", &buf, &size, NULL, 0) == 0) {
		temp = ((float)buf-2732)/10;
	}
#endif
	return temp;
}
