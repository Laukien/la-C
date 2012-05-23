#include "la_database_postgresql.h"
#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	DATABASE_POSTGRESQL *db = database_postgresql_new();
	if (error_exists()) {
		error_show();
		exit (1);
	}

	database_postgresql_setHost(db, "localhost");
	database_postgresql_setName(db, "template1");
	database_postgresql_setUser(db, "postgres");
	database_postgresql_setPassword(db, "postgres");

	database_postgresql_open(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	char *datname;
	char *datdba;
	char *encoding;
	char *datistemplate;
	char *datallowconn;
	database_postgresql_execute(db, "SELECT * FROM pg_database WHERE datname NOT LIKE '%?%' AND datdba < ?;", "template", 100);
	printf ( "| %15s | %15s | %15s | %15s | %15s |\n", "datname", "datdba", "encoding", "datistemplate", "datallowconn" );
	printf ( "|-----------------+-----------------+-----------------+-----------------+-----------------|\n" );
	while (database_postgresql_nextResult(db)) {
		datname = database_postgresql_getString(db, 0);
		datdba = database_postgresql_getString(db, 1);
		encoding = database_postgresql_getString(db, 2);
		datistemplate = database_postgresql_getString(db, 3);
		datallowconn = database_postgresql_getString(db, 4);

		printf ( "| %15s | %15s | %15s | %15s | %15s |\n", datname, datdba, encoding, datistemplate, datallowconn );

		free(datname);
		free(datdba);
		free(encoding);
		free(datistemplate);
		free(datallowconn);
	}
	if (error_exists()) {
		error_show();
		exit (1);
	}

	database_postgresql_close(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	database_postgresql_free(db);
	if (error_exists()) {
		error_show();
		exit (1);
	}

	return 0;
}
