#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	if (system_isCopyright()) {
		printf ( "(c) 2011 - 2014 by Stephan Laukien\n" );

		printf ( "\n" );
		printf ( "Set the environment variable 'COPYRIGHT' to 'false', 'no' or '0' to hide this copyright information.\n" );
	} else {
		printf ( "No copyright!\n" );
		printf ( "\n" );
		printf ( "Remove the environment variable 'COPYRIGHT' or set them to 'true', 'yes' or '1' to show this copyright information.\n" );
	}

	return 0;
}
