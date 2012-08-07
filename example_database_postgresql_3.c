#include "la_database_postgresql.h"
#include "la_exception.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	EXCEPTION *e;
	DATABASE_POSTGRESQL *db;
	char *ver;
} DATA;

void except(int id, const char *msg, void *ptr) {
	fprintf (stderr, "\nAn exception has been thrown: %d - %s\n", id, msg );

	DATA *data = (DATA *)ptr;
	if (data->ver != NULL) {
		free(data->ver);
		data->ver = NULL;
	}
	if (data->db != NULL) {
		database_postgresql_free(data->db);
		data->db = NULL;
	}
	if (data->e != NULL) {
		exception_free(data->e);
		data->e = NULL;
	}
	exit(EXIT_FAILURE);
}

int main(void) {
	DATA data;
	data.e = NULL;
	data.db = NULL;
	data.ver = NULL;

	printf ( "EXCEPTION\n" );
	data.e = exception_new();
	if (data.e == NULL) {
		fprintf(stderr, "Unable to init exception\n");
		return(EXIT_FAILURE);
	}
	exception_addCallback(data.e, except, &data);

	printf ( "NEW\n" );
	data.db = database_postgresql_new();
	if (data.db == NULL) {
		fprintf(stderr, "Unable to init database\n");
		return(EXIT_FAILURE);
	}
	database_setException(data.db, data.e);

	printf ( "PARAM\n" );
	database_postgresql_setHost(data.db, "localhosxt");
	database_postgresql_setName(data.db, "template1");
	database_postgresql_setUser(data.db, "postgres");
	database_postgresql_setPassword(data.db, "postgres");

	printf ( "OPEN\n" );
	database_postgresql_open(data.db);

	printf ( "VERSION" );
	data.ver = database_postgresql_getVersion(data.db);
	printf ( " (%s)\n", data.ver );
	free(data.ver);

	printf ( "CLOSE\n" );
	database_postgresql_close(data.db);

	printf ( "FREE\n" );
	database_postgresql_free(data.db);

	return 0;
}
