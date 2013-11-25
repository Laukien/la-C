#include <stdlib.h>
#include <stdio.h>
#include "la_string.h"

int main(void) {

	char *text = " Text to trim...with spaces...  ";
	char *trim = string_trim(text);

	printf ( "text: >%s<\n", text );
	printf ( "trim: >%s<\n", trim );

	free(trim);

	exit (EXIT_SUCCESS);
}
