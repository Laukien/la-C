#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* OS */
	printf ( "OS:\t" );
	if (system_isOSTypeWindows()) printf ( "Windows" );
	else if (system_isOSTypeLinux()) printf ( "Linux" );
	else printf ( "Unknown" );
	printf ( "\n" );

#ifndef SYSTEM_OS_TYPE_WINDOWS
	/* Uptime */
	SYSTEM_UPTIME ut;
	ut = system_getUptime();
	printf ( "Uptime: %d.%d.%d.%d.%d\n",ut.day, ut.hour, ut.minute, ut.second, ut.millisecond );
#endif
	return EXIT_SUCCESS;

}
