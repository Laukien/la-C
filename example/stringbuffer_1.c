#include "la_stringbuffer.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    STRINGBUFFER *str = stringbuffer_new();        /* init string */

	stringbuffer_append(str, "Test");                 /* append string */
	stringbuffer_append(str, "!");
	char *text;
	text = stringbuffer_text(str);
	printf ( "STRING: %s\n", text );
	free(text);

	stringbuffer_insert(str, "Das ist", 0);           /* insert string */
	printf ( "STRING: %s\n", stringbuffer_getTextPointer(str));

	stringbuffer_insert(str, " ", 7);
	printf ( "STRING: %s\n", stringbuffer_getTextPointer(str));

	stringbuffer_insert(str, "!!", stringbuffer_getLength(str));
	printf ( "STRING: %s\n", stringbuffer_getTextPointer(str));

	printf ( "SIZE: %d\n", stringbuffer_size(str) );  /* show string-size */

	stringbuffer_free(str);                           /* free resources */
	return(0);
}
