#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "la_exception.h"


/*-----------------------------------------------------------------------------
 *  data
 *-----------------------------------------------------------------------------*/
typedef struct {
	char text[10];
} DATA;

/*-----------------------------------------------------------------------------
 *  callback
 *-----------------------------------------------------------------------------*/
void mainException(int id, const char *msg, void *ptr) {
	printf ( "MAIN: %d - %s\n", id, msg );
	if (ptr != NULL) {
		printf ( "DATA: %s\n", ((DATA *)ptr)->text );
	}
}


/*-----------------------------------------------------------------------------
 *  main
 *-----------------------------------------------------------------------------*/
int main() {
	/* init */
	EXCEPTION *e = exception_new();

	/* simple */
	exception_addCallback(e, mainException, NULL);
	exception_throw(e, 1, "Eins!");
	exception_delCallback(e);

	/* data  */
	DATA data;
	strcpy(data.text, "Test");
	exception_addCallback(e, mainException, &data);
	exception_throw(e, 2, "Zwei!");
	exception_delCallback(e);

	/* none */
	exception_throw(e, 3, "Drei!");

	/* free */
	exception_free(e);

	return 0;
}
