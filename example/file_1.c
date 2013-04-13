#include "la_file.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	
	char *tmp;
	tmp = file_temp();
	printf ( "TEMP-FILE: %s\n", tmp );
	free(tmp);

	return 0;
}
