#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "la_database_postgresql.h"
#include "la_stringbuffer.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define SQL_NEXTID "SELECT nextval('system.session_id_seq'::regclass)"
#define SQL_RANDOM "SELECT (random()*65535)::int"
#define SQL_VERSION "SELECT version()"

char *database_postgresql_escapeParameter(const char *query) {
	char *result = (char *)malloc(strlen(query)*2+1);
	PQescapeString(result, query, strlen(query));

	return result;
}

BOOL database_postgresql_checkSelf(DATABASE_POSTGRESQL *self) {
	error_reset();
	if (self == NULL) {
		error_set(DATABASE_POSTGRESQL_ERROR_INIT, "database not initiated");
		return FALSE;
	}

	return TRUE;
}

BOOL database_postgresql_checkParameter(DATABASE_POSTGRESQL *self) {
	if (
		self->host == NULL
		||
		self->port <= 0
		||
		self->user == NULL
		||
		self->password == NULL
		||
		self->name == NULL
	) {
		error_set(DATABASE_POSTGRESQL_ERROR_PARAMETER, "invalid parameter");
		return FALSE;
	}

	return TRUE;
}

DATABASE_POSTGRESQL *database_postgresql_new() {
	error_init();
	DATABASE_POSTGRESQL *self = (DATABASE_POSTGRESQL *)malloc(sizeof(DATABASE_POSTGRESQL));
	if (self == NULL) {
		error_set(DATABASE_POSTGRESQL_ERROR_MEMORY, "init memory");
	}

	self->host = NULL;
	self->port = 5432;
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

void database_postgresql_free(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return;

	/* close objects */
	if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);
	if (database_postgresql_isOpen(self)) database_postgresql_close(self);

	/* free strings */
	if (self->host != NULL) free (self->host);
	self->port = 5432;
	if (self->name != NULL) free (self->name);
	if (self->user != NULL) free (self->user);
	if (self->password != NULL) free (self->password);
	if (self->schema != NULL) free (self->schema);

	/* free database */
	free (self);
}

void database_postgresql_open(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return;
	if (!database_postgresql_checkParameter(self)) return;

	if (database_postgresql_isOpen(self)) {
		error_set(DATABASE_POSTGRESQL_ERROR_CONNECTION, "connection already opened");
		return;
	}

	char conn[DATABASE_POSTGRESQL_CONNECTION_SIZE + 1];
	snprintf(conn, DATABASE_POSTGRESQL_CONNECTION_SIZE, "host=%s port=%d dbname=%s user=%s password=%s", self->host, self->port, self->name, self->user, self->password);
	self->connection = PQconnectdb(conn);
}

void database_postgresql_close(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return;

	if (database_postgresql_isOpen(self)) {
		if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);
		PQfinish(self->connection);
		self->connection = NULL;
	} else {
		error_set(DATABASE_POSTGRESQL_ERROR_CONNECTION, "connection already closed");
	}
}

BOOL database_postgresql_isOpen(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return FALSE;

	return self->connection != NULL;
}

void database_postgresql_setHost(DATABASE_POSTGRESQL *self, const char *host) {
	if (!database_postgresql_checkSelf(self)) return;

	self->host = (char *)malloc(strlen(host) + 1);
	strcpy(self->host, host);
}

void database_postgresql_setPort(DATABASE_POSTGRESQL *self, int port) {
	if (!database_postgresql_checkSelf(self)) return;

	self->port = port;
}

void database_postgresql_setName(DATABASE_POSTGRESQL *self, const char *name) {
	if (!database_postgresql_checkSelf(self)) return;

	self->name = (char *)malloc(strlen(name) + 1);
	strcpy(self->name, name);
}

void database_postgresql_setUser(DATABASE_POSTGRESQL *self, const char *user) {
	if (!database_postgresql_checkSelf(self)) return;

	self->user = (char *)malloc(strlen(user) + 1);
	strcpy(self->user, user);
}

void database_postgresql_setPassword(DATABASE_POSTGRESQL *self, const char *password) {
	if (!database_postgresql_checkSelf(self)) return;

	self->password = (char *)malloc(strlen(password) + 1);
	strcpy(self->password, password);
}

void database_postgresql_setSchema(DATABASE_POSTGRESQL *self, const char *schema) {
	if (!database_postgresql_checkSelf(self)) return;

	self->schema = (char *)malloc(strlen(schema) + 1);
	strcpy(self->schema, schema);
}

void database_postgresql_closeResult(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return;

	if (!database_postgresql_isResult(self)) {
		error_set(DATABASE_POSTGRESQL_ERROR_RESULT, "result is closed");
		return;
	}

	PQclear(self->result);
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
}

BOOL database_postgresql_isResult(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return FALSE;

	return self->result != NULL;
}

BOOL database_postgresql_nextResult(DATABASE_POSTGRESQL *self) {
	if (database_postgresql_isResult(self) && self->resultCur < (self->resultRow) - 1) {
		++(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_postgresql_previousResult(DATABASE_POSTGRESQL *self) {
	if (database_postgresql_isResult(self) && self->resultCur > 0) {
		--(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_postgresql_firstResult(DATABASE_POSTGRESQL *self) {
	if (database_postgresql_isResult(self)) {
		self->resultCur = 0;

		return TRUE;
	} else return FALSE;
}

BOOL database_postgresql_lastResult(DATABASE_POSTGRESQL *self) {
	if (database_postgresql_isResult(self)) {
		self->resultCur = self->resultRow - 1;

		return TRUE;
	} else return FALSE;
}

int database_postgresql_getResultRowCount(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return 0;

	return self->resultRow;
}

int database_postgresql_getResultColumnCount(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return 0;

	return self->resultCol;
}

int database_postgresql_getResultCursor(DATABASE_POSTGRESQL *self) {
	if (!database_postgresql_checkSelf(self)) return 0;

	return self->resultCur;
}

char *database_postgresql_getString(DATABASE_POSTGRESQL *self, int col) {
	if (!database_postgresql_isResult(self)) return NULL;
	if (self->resultCur < 0 || self->resultCur >= self->resultRow) {
		error_set(DATABASE_POSTGRESQL_ERROR_PARAMETER, "row out of range");
		return NULL;
	}
	if (col < 0 || col >= self->resultCol) {
		error_set(DATABASE_POSTGRESQL_ERROR_PARAMETER, "column out of range");
		return NULL;
	}
	if (PQbinaryTuples(self->result)) {
		error_set(DATABASE_POSTGRESQL_ERROR_FORMAT, "data format");
		return NULL;
	}
	
	if (PQgetisnull(self->result, self->resultCur, col)) return NULL;
	char *dbres = PQgetvalue(self->result, self->resultCur, col);
	char *res = (char *)malloc(strlen(dbres) + 1);
	if(res == NULL) {
		error_set(DATABASE_POSTGRESQL_ERROR_MEMORY, "out of memory");
		return NULL;
	}
	strcpy(res, dbres);

	return res;
}

void database_postgresql_execute(DATABASE_POSTGRESQL *self, const char *query, ...) {
	if (!database_postgresql_checkSelf(self)) return;
		
	/* reset */
	if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);

	/* query */
	char *text = (char*) malloc(strlen(query) + 1);
	strcpy(text, query);

	va_list args;
	va_start(args, query);

	STRINGBUFFER *sb;
	sb = stringbuffer_new();

	int i;
	int idx = 0;
	BOOL isString = FALSE;
	char fmt;
	for (i = 0; text[i] != '\0'; ++i) {
		if (text[i] == '\'') {
			isString = !isString;
			continue;
		}
		if (text[i] != '?') continue;

		text[i] = '\0';
		stringbuffer_append(sb, text + idx);

		++i;
		fmt = text[i];
		if (isString) {
			char *val = va_arg(args, char *);
			char *esc = database_postgresql_escapeParameter(val);
			stringbuffer_append(sb, esc);
			free(esc);
		} else {
			int val = va_arg(args, int);
			char esc[DATABASE_POSTGRESQL_NUMBER_SIZE + 1];
			sprintf(esc, "%d", val);
			stringbuffer_append(sb, esc);
		}

        idx = i;                                /* set idx-start to next index */
	}
	stringbuffer_append(sb, text + idx);

	va_end(args);

	self->result = PQexec(self->connection, stringbuffer_text(sb));
	stringbuffer_free(sb);
	free(text);
	if (self->result == NULL) {
		error_set(DATABASE_POSTGRESQL_ERROR_EXECUTE, "unable to execute");
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
			error_set(DATABASE_POSTGRESQL_ERROR_EXECUTE, PQresultErrorMessage(self->result));
	}	
}

char *database_postgresql_getVersion(DATABASE_POSTGRESQL *self) {
	database_postgresql_execute(self, SQL_VERSION);

	if (!database_postgresql_isResult(self)) return NULL;
	if (!database_postgresql_nextResult(self)) return NULL;
	return database_postgresql_getString(self, 0);
}
