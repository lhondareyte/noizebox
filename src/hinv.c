/*
 * $Id$
 * (c)2013 - Luc Hondareyte <luc.hondareyte@laposte.net>
 */

#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include "noizebox.h"

uint64_t NZ_get_free_memory(void)
{
#ifdef __FreeBSD__
        int pagesize=0;
        int freepages=0;
        size_t size;
        size = sizeof freepages;
        sysctlbyname("vm.stats.vm.v_free_count", &freepages, &size, NULL, 0);
        size = sizeof pagesize;
        sysctlbyname("vm.stats.vm.v_page_size", &pagesize, &size, NULL, 0);
	return freepages * pagesize / 1024 ;
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
	 * Conversion Kelvin -> Centigrade
	 */
	return ((float)buf-2732)/10;
#endif
	return 0.0;
}
