#include "la_database.h"
#include "la_exception.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	EXCEPTION *e = exception_new();

	printf ( "NEW\n" );
	DATABASE *db = database_new();
	database_setException(db, e);
	exception_throw(e);

	printf ( "PARAM\n" );
	database_setHost(db, "localhost");
	database_setName(db, "template1");
	database_setUser(db, "postgres");
	database_setPassword(db, "postgres");

	printf ( "OPEN\n" );
	database_open(db);
	exception_throw(e);

	printf ( "VERSION" );
	char *ver = database_getVersion(db);
	exception_throw(e);
	printf ( " (%s)\n", ver );
	free(ver);

	printf ( "RANDOM" );
	int rnd = database_getRandom(db);
	exception_throw(e);
	printf ( " (%d)\n", rnd );

	printf ( "CLOSE\n" );
	database_close(db);
	exception_throw(e);

	printf ( "FREE\n" );
	database_free(db);
	exception_throw(e);

	exception_free(e);

	return 0;
}
