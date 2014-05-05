#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int i;

	printf ( "Sleep 3 seconds:\n" );

	printf ( "\t1 \n" );
	system_sleep(1000);

	printf ( "\t2 " );
	fflush(stdout);
	for (i = 0; i < 10; ++i) {
		system_sleep(100);
		printf ( "." );
		fflush(stdout);
	}
	printf ( "\n" );

	printf ( "\t3 " );
	fflush(stdout);
	for (i = 0; i < 100; ++i) {
		system_sleep(10);
		printf ( "." );
		fflush(stdout);
	}
	printf ( "\n" );

	return 0;
}
