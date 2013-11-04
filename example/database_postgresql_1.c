#include "la_database.h"
#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	ERROR *err = error_new();

	printf ( "NEW\n" );
	DATABASE *db = database_new();
	database_setError(db, err);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	printf ( "PARAM\n" );
	database_setHost(db, "localhost");
	database_setName(db, "template1");
	database_setUser(db, "postgres");
	database_setPassword(db, "postgres");

	printf ( "OPEN\n" );
	database_open(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	printf ( "VERSION" );
	char *ver = database_getVersion(db);
	if (error_exists(err)) {
		printf ( "\n" );
		error_show(err);
		error_free(err);
		exit (1);
	}
	printf ( " (%s)\n", ver );
	free(ver);

	printf ( "CLOSE\n" );
	database_close(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	printf ( "FREE\n" );
	database_free(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	error_free(err);

	return 0;
}
