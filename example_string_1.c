#include "la_string.h"
#include <stdio.h>

int main(void) {
	LA_STRING *str = string_new();              /* init string */

	string_append(str, "Test");                 /* append string */
	string_append(str, "!");
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, "Das ist", 0);           /* insert string */
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, " ", 7);
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, "!!", string_size(str));
	printf ( "STRING: %s\n", string_pointer(str) );

	printf ( "SIZE: %d\n", string_size(str) );  /* show string-size */

	string_free(str);                           /* free resources */
	return(0);
}
