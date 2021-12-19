/*
 * Copyright (c)2013-2017, Luc Hondareyte
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

#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>

#include "global.h"

uint64_t NZ_get_free_memory(void)
{
#ifdef __FreeBSD__
        int pagesize=0;
        int free=0;
        int inactive=0;
        size_t size;
        size = sizeof free;
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
#ifdef __FreeBSD__
        size_t size;
        int buf;
        size = sizeof buf;
        if (sysctlbyname("hw.acpi.thermal.tz0.temperature", &buf, &size, NULL, 0) == -1 )
	return 0.0;
	;
	/*
	 * Conversion Kelvin -> Celsius
	 */
	return ((float)buf-2732)/10;
#endif
	return 0.0;
}
