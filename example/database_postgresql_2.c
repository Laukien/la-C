#include <stdio.h>
#include <stdlib.h>
#include "la_database_postgresql.h"
#include "la_error.h"

int main(void) {

	ERROR *err = error_new();
	DATABASE *db = database_new();
	database_setError(db, err);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	database_setHost(db, "localhost");
	database_setName(db, "template1");
	database_setUser(db, "postgres");
	database_setPassword(db, "postgres");

	database_open(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	char *datname;
	char *datdba;
	char *encoding;
	char *datistemplate;
	char *datallowconn;
	database_execute(db, "SELECT * FROM pg_database WHERE datname NOT LIKE '%?%' AND datdba < ?;", "template", 100);
	printf ( "| %15s | %15s | %15s | %15s | %15s |\n", "datname", "datdba", "encoding", "datistemplate", "datallowconn" );
	printf ( "|-----------------+-----------------+-----------------+-----------------+-----------------|\n" );
	while (database_nextResult(db)) {
		datname = database_getString(db, 0);
		datdba = database_getString(db, 1);
		encoding = database_getString(db, 2);
		datistemplate = database_getString(db, 3);
		datallowconn = database_getString(db, 4);

		printf ( "| %15s | %15s | %15s | %15s | %15s |\n", datname, datdba, encoding, datistemplate, datallowconn );

		free(datname);
		free(datdba);
		free(encoding);
		free(datistemplate);
		free(datallowconn);
	}
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	database_close(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	database_free(db);
	if (error_exists(err)) {
		error_show(err);
		error_free(err);
		exit (1);
	}

	error_free(err);
	return 0;
}
