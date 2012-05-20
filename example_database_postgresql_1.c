#include "la_database_postgresql.h"
#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	printf ( "NEW\n" );
	DATABASE_POSTGRESQL *db = database_postgresql_new();
	if (error_exists()) {
		error_show();
		exit (1);
	}

	printf ( "PARAM\n" );
	database_postgresql_setHost(db, "localhost");
	database_postgresql_setName(db, "template1");
	database_postgresql_setUser(db, "postgres");
	database_postgresql_setPassword(db, "postgres");

	printf ( "OPEN\n" );
	database_postgresql_open(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	printf ( "VERSION" );
	char *ver = database_postgresql_getVersion(db);
	if (error_exists()) {
		printf ( "\n" );
		error_show();
		exit (1);
	}
	printf ( " (%s)\n", ver );
	free(ver);

	printf ( "CLOSE\n" );
	database_postgresql_close(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	printf ( "FREE\n" );
	database_postgresql_free(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	return 0;
}
