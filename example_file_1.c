#include "la_file.h"
#include "la_number.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	
	char *tmp;
	tmp = file_temp();
	printf ( "TEMP-FILE: %s\n", tmp );
	free(tmp);

	number_uintToString(1234);
	return 0;
}
