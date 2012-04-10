#include "la_stringbuffer.h"
#include <stdio.h>

int main(void) {
    LA_STRINGBUFFER *str = stringbuffer_new();        /* init string */

	stringbuffer_append(str, "Test");                 /* append string */
	stringbuffer_append(str, "!");
	printf ( "STRING: %s\n", stringbuffer_pointer(str) );

	stringbuffer_insert(str, "Das ist", 0);           /* insert string */
	printf ( "STRING: %s\n", stringbuffer_pointer(str) );

	stringbuffer_insert(str, " ", 7);
	printf ( "STRING: %s\n", stringbuffer_pointer(str) );

	stringbuffer_insert(str, "!!", stringbuffer_size(str));
	printf ( "STRING: %s\n", stringbuffer_pointer(str) );

	printf ( "SIZE: %d\n", stringbuffer_size(str) );  /* show string-size */

	stringbuffer_free(str);                           /* free resources */
	return(0);
}
