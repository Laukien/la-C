#include "la_string.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *hex = string_toHex("abcABCxyzXYZ");
	printf ( "HEX: %s\n", hex );

	char *txt = string_fromHex(hex);
	printf ( "TXT: %s\n", txt );

	free(hex);
	free(txt);

	return EXIT_SUCCESS;
}
