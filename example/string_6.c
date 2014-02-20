#include <stdio.h>
#include <stdlib.h>
#include "la_string.h"

int main(void) {
	char *camel = string_toCamel("^dieser Text ist ein Kamel!$");
	printf ( "Camel: %s\n", camel );
	free(camel);

	return EXIT_SUCCESS;
}
