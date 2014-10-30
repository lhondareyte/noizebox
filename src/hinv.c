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
        size_t size;
        int freepages, pagesize;
        size = sizeof freepages;
        sysctlbyname("vm.stats.vm.v_free_count", &freepages, &size, NULL, 0);
        size = sizeof pagesize;
        sysctlbyname("vm.stats.vm.v_page_size", &pagesize, &size, NULL, 0);
#endif
	return( freepages * pagesize / 1024 );
}

float NZ_get_cpu_temperature(void)
{
#ifdef __FreeBSD__
        size_t size;
        int buf;
        size = sizeof buf;
        sysctlbyname("hw.acpi.thermal.tz0.temperature", &buf, &size, NULL, 0);
#endif
	/*
	 * Conversion Kelvin -> Centigrade
	 */
	return (((float)buf-2732)/10);
}
