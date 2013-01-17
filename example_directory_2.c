#include "la_directory.h"
#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* create */
	directory_create("/tmp/sub/foo/bar");

	/* check */
	printf ( "directory exists: %s\n", directory_exists("/tmp/sub/foo/bar") ? "TRUE" : "FALSE" );

	/* remove */
//	directory_delete("/tmp/sub/foo/bar");

	return 0;
}
