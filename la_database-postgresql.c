#include "la_database-postgresql.h"
#include <stdlib.h>
#include <string.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define SQL_NEXTID "SELECT nextval('system.session_id_seq'::regclass)"
#define SQL_RANDOM "SELECT (random()*65535)::int"
#define SQL_VERSION "SELECT version()"

char *escapeParameter(const char *query) {
	char *result = (char *)malloc(strlen(query)*2+1);
	PQescapeString(result, query, strlen(query));

	return result;
}


BOOL checkParameter(LA_DATABASE *self) {
	if (
		self->host == NULL
		||
		self->user == NULL
		||
		self->password == NULL
		||
		self->name == NULL
	) {
		error_set(LA_DATABASE_ERROR_PARAMETER, "invalid parameter");
		return FALSE;
	}

	return TRUE;
}

LA_DATABASE *database_new() {
	error_init();
	LA_DATABASE *self = (LA_DATABASE *)malloc(sizeof(LA_DATABASE));
	if (self == NULL) {
		error_set(LA_DATABASE_ERROR_MEMORY, "init memory");
	}

	self->host = NULL;
	self->name = NULL;
	self->user = NULL;
	self->password = NULL;
	self->schema = NULL;
	self->connection = NULL;
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;

	return self;
}

void database_free(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return;

	/* close objects */
	if (database_isResult(self)) database_closeResult(self);
	if (database_isOpen(self)) database_close(self);

	/* free strings */
	if (self->host != NULL) free (self->host);
	if (self->name != NULL) free (self->name);
	if (self->user != NULL) free (self->user);
	if (self->password != NULL) free (self->password);
	if (self->schema != NULL) free (self->schema);

	/* free database */
	free (self);
}

void database_open(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return;
	if (!checkParameter(self)) return;

	if (database_isOpen(self)) {
		error_set(LA_DATABASE_ERROR_CONNECTION, "connection already opened");
		return;
	}

	char conn[512 + 1];
	snprintf(conn, 512, "host=%s dbname=%s user=%s password=%s", self->host, self->name, self->user, self->password);
	self->connection = PQconnectdb(conn);
}

void database_close(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return;

	if (database_isOpen(self)) {
		if (database_isResult(self)) database_closeResult(self);
		PQfinish(self->connection);
		self->connection = NULL;
	} else {
		error_set(LA_DATABASE_ERROR_CONNECTION, "connection already closed");
	}
}

BOOL database_isOpen(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return FALSE;

	return self->connection != NULL;
}

void database_closeResult(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return;

	if (!database_isResult(self)) {
		error_set(LA_DATABASE_ERROR_RESULT, "result is closed");
		return;
	}

	PQclear(self->result);
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
}

BOOL database_isResult(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return FALSE;

	return self->result != NULL;
}

BOOL database_nextResult(LA_DATABASE *self) {
	if (database_isResult(self) && self->resultCur < self->resultRow) {
		++(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_previousResult(LA_DATABASE *self) {
	if (database_isResult(self) && self->resultCur > 0) {
		--(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_firstResult(LA_DATABASE *self) {
	if (database_isResult(self)) {
		self->resultCur = 0;

		return TRUE;
	} else return FALSE;
}

BOOL database_lastResult(LA_DATABASE *self) {
	if (database_isResult(self)) {
		self->resultCur = self->resultRow - 1;

		return TRUE;
	} else return FALSE;
}

char *database_getString(LA_DATABASE *self, int col) {
	if (!database_isResult(self)) return NULL;
	if (self->resultCur < 0 || self->resultCur >= self->resultRow) {
		error_set(LA_DATABASE_ERROR_PARAMETER, "row out of range");
		return NULL;
	}
	if (col < 0 || col >= self->resultCol) {
		error_set(LA_DATABASE_ERROR_PARAMETER, "column out of range");
		return NULL;
	}
	if (PQbinaryTuples(self->result)) {
		error_set(LA_DATABASE_ERROR_FORMAT, "data format");
		return NULL;
	}
	
	if (PQgetisnull(self->result, self->resultCur, col)) return NULL;
	char *dbres = PQgetvalue(self->result, self->resultCur, col);
	char *res = (char *)malloc(strlen(dbres) + 1);
	if(res == NULL) {
		error_set(LA_DATABASE_ERROR_MEMORY, "out of memory");
		return NULL;
	}
	strcpy(res, dbres);

	return res;
}

void database_execute(LA_DATABASE *self, const char *query, ...) {
	if (!database_checkSelf(self)) return;
		
	/* reset */
	if (database_isResult(self)) database_closeResult(self);

	self->result = PQexec(self->connection, query);
	if (self->result == NULL) {
		error_set(LA_DATABASE_ERROR_EXECUTE, "unable to execute");
		return;
	}

	switch(PQresultStatus(self->result)) {
		case PGRES_COMMAND_OK:
			return;
		case PGRES_TUPLES_OK:
			self->resultRow = PQntuples(self->result);
			self->resultCol = PQnfields(self->result);
			return;
		default:
			error_set(LA_DATABASE_ERROR_EXECUTE, PQresultErrorMessage(self->result));
	}	
}

char *database_getVersion(LA_DATABASE *self) {
	database_execute(self, SQL_VERSION);

	if (!database_isResult(self)) return NULL;
	if (!database_nextResult(self)) return NULL;
	return database_getString(self, 0);
}
