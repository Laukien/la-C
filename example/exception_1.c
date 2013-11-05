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
void mainException(EXCEPTION *e, void *ptr) {
	exception_show(e);

	if (ptr != NULL) {
		printf ( "     DATA:\t%s\n", ((DATA *)ptr)->text );
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
	exception_setShort(e, 1, "This is a short message.");
	exception_throwCallback(e);
	exception_delCallback(e);

	/* clean */
	exception_reset(e);

	/* data  */
	DATA data;
	strcpy(data.text, "Test");
	exception_addCallback(e, mainException, &data);
	exception_setLong(e, 2, "This is a long message.", "...to learn...", "Learn!");
	exception_throwCallback(e);
	exception_delCallback(e);

	/* clean */
	exception_reset(e);

	/* none */
	exception_setShort(e, 3, "Count: %c, %d, %s", '1', 2, "3");
	exception_throwCallback(e);                 /* nothing will happen */
	exception_show(e);

	/* free */
	exception_free(e);

	return 0;
}
