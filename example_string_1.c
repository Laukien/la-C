#include "la_string.h"
#include <stdio.h>

int main(void) {
	LA_STRING *str = string_new();

	string_append(str, "Test");
	string_append(str, "!");
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, "Das ist", 0);
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, " ", 7);
	printf ( "STRING: %s\n", string_pointer(str) );

	string_insert(str, "!!", string_size(str));
	printf ( "STRING: %s\n", string_pointer(str) );
	printf ( "SIZE: %d\n", string_size(str) );

	return(0);
}
