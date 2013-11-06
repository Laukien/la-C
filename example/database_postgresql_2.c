#include <stdio.h>
#include <stdlib.h>
#include "la_database.h"
#include "la_exception.h"

int main(void) {
	EXCEPTION *e = exception_new();

	DATABASE *db = database_new();
	database_setException(db, e);
	exception_throw(e);

	database_setHost(db, "localhost");
	database_setName(db, "template1");
	database_setUser(db, "postgres");
	database_setPassword(db, "postgres");

	database_open(db);
	exception_throw(e);

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
	exception_throw(e);

	database_close(db);
	exception_throw(e);

	database_free(db);
	exception_throw(e);

	exception_free(e);
	return 0;
}
