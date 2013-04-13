#include "la_file.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
	size_t size = file_size(argv[0]);
	printf ( "FILE-SIZE: %u\n", (unsigned int)size );

	return 0;
}
