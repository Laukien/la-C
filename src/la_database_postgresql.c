#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "la_database_postgresql.h"
#include "la_error.h"
#include "la_message.h"
#include "la_stringbuffer.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define SQL_NEXTID "SELECT nextval('system.session_id_seq'::regclass)"
#define SQL_RANDOM "SELECT (random()*65535)::int"
#define SQL_VERSION "SELECT version()"

struct la_database_postgresql {
	ERROR *error;
	EXCEPTION *exception;
	char *host;
	int port;
	char *name;
	char *user;
	char *password;
	char *schema;
	int resultCol;
	int resultRow;
	int resultCur;
	PGconn *connection;
	PGresult *result;
};

void database_postgresql_throwError(DATABASE_POSTGRESQL *self, int id, const char *message, const char *cause, const char *action) {
		assert(self);

		if (self->error) {
			error_set(self->error, id, message);
			error_setComment(self->error, "database_postgresql");
			error_setCause(self->error, cause);
			error_setAction(self->error, action);
		}

		if (self->exception) {
			exception_throw(self->exception, id, message);
		}
}
	
char *database_postgresql_escapeParameter(const char *query) {
	char *result = (char *)malloc(strlen(query) * 2 + 1);
	PQescapeString(result, query, strlen(query));

	return result;
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
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_PARAMETER, "wrong or un-set parameter", "invalid parameter", "check parameters");
		return FALSE;
	}

	return TRUE;
}

DATABASE_POSTGRESQL *database_postgresql_new() {
	DATABASE_POSTGRESQL *self = (DATABASE_POSTGRESQL *)malloc(sizeof(DATABASE_POSTGRESQL));
	if (!self) {
		message_error("unable to get memory");
	}

	self->exception = NULL;
	self->error = NULL;
	self->host = NULL;
	self->port = DATABASE_POSTGRESQL_PORT;
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

void database_postgresql_setError(DATABASE_POSTGRESQL *self, ERROR *e) {
	assert(self != NULL);
	assert(e != NULL);

	self->error = e;
}

void database_postgresql_setException(DATABASE_POSTGRESQL *self, EXCEPTION *e) {
	assert(self != NULL);
	assert(e != NULL);

	self->exception = e;
}

void database_postgresql_free(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	/* close objects */
	if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);
	if (database_postgresql_isOpen(self)) database_postgresql_close(self);

	/* free strings */
	if (self->host != NULL) free (self->host);
	self->port = DATABASE_POSTGRESQL_PORT;
	if (self->name != NULL) free (self->name);
	if (self->user != NULL) free (self->user);
	if (self->password != NULL) free (self->password);
	if (self->schema != NULL) free (self->schema);

	/* free database */
	free (self);
}

void database_postgresql_open(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);
	if (!database_postgresql_checkParameter(self)) return;

	if (database_postgresql_isOpen(self)) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_CONNECTION, "unable to open connection", "connection already opened", "check for an other \"open\"");
		return;
	}

	char conn[DATABASE_POSTGRESQL_CONNECTION_SIZE + 1];
	snprintf(conn, DATABASE_POSTGRESQL_CONNECTION_SIZE, "host=%s port=%d dbname=%s user=%s password=%s", self->host, self->port, self->name, self->user, self->password);
	self->connection = PQconnectdb(conn);
	if (PQstatus(self->connection) == CONNECTION_BAD) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_CONNECTION, "unable to open database", PQerrorMessage(self->connection), "check connection parameters");
	}
}

void database_postgresql_close(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	if (database_postgresql_isOpen(self)) {
		if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);
		PQfinish(self->connection);
		self->connection = NULL;
	} else {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_CONNECTION, "unable to close connection", "connection already closed", "check for an other \"close\"");
	}
}

BOOL database_postgresql_isOpen(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	return self->connection != NULL;
}

void database_postgresql_setHost(DATABASE_POSTGRESQL *self, const char *host) {
	assert(self != NULL);
	assert(host != NULL);

	self->host = strdup(host);
}

void database_postgresql_setPort(DATABASE_POSTGRESQL *self, int port) {
	assert(self != NULL);
	assert(port > 0);

	self->port = port;
}

void database_postgresql_setName(DATABASE_POSTGRESQL *self, const char *name) {
	assert(self != NULL);
	assert(name != NULL);

	self->name = strdup(name);
}

void database_postgresql_setUser(DATABASE_POSTGRESQL *self, const char *user) {
	assert(self != NULL);
	assert(user != NULL);

	self->user = strdup(user);
}

void database_postgresql_setPassword(DATABASE_POSTGRESQL *self, const char *password) {
	assert(self != NULL);
	assert(password != NULL);

	self->password = strdup(password);
}

void database_postgresql_setSchema(DATABASE_POSTGRESQL *self, const char *schema) {
	assert(self != NULL);
	assert(schema != NULL);

	self->schema = strdup(schema);
}

void database_postgresql_closeResult(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	if (!database_postgresql_isResult(self)) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_RESULT, "unable to close result", "result has been closed already", "check for an other \"close\"");
		return;
	}

	PQclear(self->result);
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
}

BOOL database_postgresql_isResult(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

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
	assert(self != NULL);

	return self->resultRow;
}

int database_postgresql_getResultColumnCount(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	return self->resultCol;
}

int database_postgresql_getResultCursor(DATABASE_POSTGRESQL *self) {
	assert(self != NULL);

	return self->resultCur;
}

char *database_postgresql_getString(DATABASE_POSTGRESQL *self, int col) {
	if (!database_postgresql_isResult(self)) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_RESULT, "unable to get result", "no result has been set before", "check your query or my you don't asked for a result");
		return NULL;
	}
	if (self->resultCur < 0 || self->resultCur >= self->resultRow) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_PARAMETER, "unable to get result", "row out of range", "check the row count");
		return NULL;
	}
	if (col < 0 || col >= self->resultCol) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_PARAMETER, "unable to get result", "column out of range", "check for column count");
		return NULL;
	}
	if (PQbinaryTuples(self->result)) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_FORMAT, "unable to get result", "the data is nopt a string", "check the the format");
		return NULL;
	}
	
	if (PQgetisnull(self->result, self->resultCur, col)) return NULL;
	char *dbres = PQgetvalue(self->result, self->resultCur, col);
	char *res = (char *)malloc(strlen(dbres) + 1);
	if(res == NULL) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_MEMORY, "unable to get memory", "out of memory", "get more memory or redesign your query");
		return NULL;
	}
	strcpy(res, dbres);

	return res;
}

void database_postgresql_execute(DATABASE_POSTGRESQL *self, const char *query, ...) {
	assert(self != NULL);
		
	/* reset */
	if (database_postgresql_isResult(self)) database_postgresql_closeResult(self);

	/* query */
	char *text = strdup(query);

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
	free(text);

	self->result = PQexec(self->connection, stringbuffer_getTextPointer(sb));
	stringbuffer_free(sb);
	if (self->result == NULL) {
		database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_EXECUTE, "no result", "unable to get result from database", "check your query");
		return;
	}

	switch(PQresultStatus(self->result)) {
		case PGRES_COMMAND_OK:
			self->resultRow = 0;
			self->resultCol = 0;
			return;
		case PGRES_TUPLES_OK:
			self->resultRow = PQntuples(self->result);
			self->resultCol = PQnfields(self->result);
			return;
		default:
			database_postgresql_throwError(self, DATABASE_POSTGRESQL_ERROR_EXECUTE, "invalid status", PQresultErrorMessage(self->result), "check the query");
	}	
}

char *database_postgresql_getVersion(DATABASE_POSTGRESQL *self) {
	database_postgresql_execute(self, SQL_VERSION);

	if (!database_postgresql_isResult(self)) return NULL;
	if (!database_postgresql_nextResult(self)) return NULL;
	return database_postgresql_getString(self, 0);
}
