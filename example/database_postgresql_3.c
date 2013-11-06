#include <stdio.h>
#include <stdlib.h>
#include "la_database.h"
#include "la_exception.h"

typedef struct {
	EXCEPTION *e;
	DATABASE *db;
	char *ver;
} DATA;

void except(EXCEPTION *e, void *ptr) {
	exception_show(e);

	DATA *data = (DATA *)ptr;
	if (data->ver != NULL) {
		free(data->ver);
		data->ver = NULL;
	}
	if (data->db != NULL) {
		database_free(data->db);
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
	data.db = database_new();
	if (data.db == NULL) {
		fprintf(stderr, "Unable to init database\n");
		return(EXIT_FAILURE);
	}
	database_setException(data.db, data.e);

	printf ( "PARAM\n" );
	database_setHost(data.db, "localhost");
	database_setName(data.db, "template1");
	database_setUser(data.db, "postgres");
	database_setPassword(data.db, "postgres");

	printf ( "OPEN\n" );
	database_open(data.db);

	printf ( "VERSION" );
	data.ver = database_getVersion(data.db);
	printf ( " (%s)\n", data.ver );
	free(data.ver);

	printf ( "CLOSE\n" );
	database_close(data.db);

	printf ( "---->FREE\n" );
	database_free(data.db);
	exception_free(data.e);

	return 0;
}
