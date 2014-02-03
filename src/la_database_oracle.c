/*
 * =====================================================================================
 *
 *       Filename:  la_database_oracle.c
 *    Description:  Oracle wrapper
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "la_database_oracle.h"
#include "la_stringbuffer.h"

#define SQL_VERSION "SELECT * FROM v$version;"

//TODO
char *database_oracle_escapeParameter(const char *query) {
	return NULL;
}

BOOL database_oracle_checkSelf(DATABASE_ORACLE *self) {
	error_reset();
	if (self == NULL) {
		error_set(DATABASE_ORACLE_ERROR_INIT, "database not initiated");
		return FALSE;
	}

	return TRUE;
}

BOOL database_oracle_checkParameter(DATABASE_ORACLE *self) {
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
		||
		self->sid == NULL
	) {
		error_set(DATABASE_ORACLE_ERROR_PARAMETER, "invalid parameter");
		return FALSE;
	}

	return TRUE;
}

DATABASE_ORACLE *database_oracle_new() {
	error_init();
	DATABASE_ORACLE *self = (DATABASE_ORACLE *)malloc(sizeof(DATABASE_ORACLE));
	if (self == NULL) {
		error_set(DATABASE_ORACLE_ERROR_MEMORY, "init memory");
	}

	self->host = NULL;
	self->port = 1521;
	self->name = NULL;
	self->user = NULL;
	self->password = NULL;
	self->schema = NULL;
	self->sid = NULL;
	self->connection = NULL;
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;

	return self;
}

void database_oracle_free(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return;

	/* close objects */
	if (database_oracle_isResult(self)) database_oracle_closeResult(self);
	if (database_oracle_isOpen(self)) database_oracle_close(self);

	/* free strings */
	if (self->host != NULL) free (self->host);
	self->port = 1521;
	if (self->name != NULL) free (self->name);
	if (self->user != NULL) free (self->user);
	if (self->password != NULL) free (self->password);
	if (self->schema != NULL) free (self->schema);

	/* free database */
	free (self);
}

void database_oracle_open(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return;
	if (!database_oracle_checkParameter(self)) return;

	if (database_oracle_isOpen(self)) {
		error_set(DATABASE_ORACLE_ERROR_CONNECTION, "connection already opened");
		return;
	}

	char conn[DATABASE_ORACLE_CONNECTION_SIZE + 1];
	snprintf(conn, DATABASE_ORACLE_CONNECTION_SIZE, "host=%s dbname=%s user=%s password=%s", self->host, self->name, self->user, self->password);
//	self->connection = PQconnectdb(conn);
}

void database_oracle_close(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return;

	if (database_oracle_isOpen(self)) {
		if (database_oracle_isResult(self)) database_oracle_closeResult(self);
//		PQfinish(self->connection);
		self->connection = NULL;
	} else {
		error_set(DATABASE_ORACLE_ERROR_CONNECTION, "connection already closed");
	}
}

BOOL database_oracle_isOpen(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return FALSE;

	return self->connection != NULL;
}

void database_oracle_setHost(DATABASE_ORACLE *self, const char *host) {
	if (!database_oracle_checkSelf(self)) return;

	self->host = (char *)malloc(strlen(host) + 1);
	strcpy(self->host, host);
}

void database_oracle_setPort(DATABASE_ORACLE *self, int port) {
	if (!database_oracle_checkSelf(self)) return;

	self->port = port;
}

void database_oracle_setName(DATABASE_ORACLE *self, const char *name) {
	if (!database_oracle_checkSelf(self)) return;

	self->name = (char *)malloc(strlen(name) + 1);
	strcpy(self->name, name);
}

void database_oracle_setUser(DATABASE_ORACLE *self, const char *user) {
	if (!database_oracle_checkSelf(self)) return;

	self->user = (char *)malloc(strlen(user) + 1);
	strcpy(self->user, user);
}

void database_oracle_setPassword(DATABASE_ORACLE *self, const char *password) {
	if (!database_oracle_checkSelf(self)) return;

	self->password = (char *)malloc(strlen(password) + 1);
	strcpy(self->password, password);
}

void database_oracle_setSchema(DATABASE_ORACLE *self, const char *schema) {
	if (!database_oracle_checkSelf(self)) return;

	self->schema = (char *)malloc(strlen(schema) + 1);
	strcpy(self->schema, schema);
}

void database_oracle_closeResult(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return;

	if (!database_oracle_isResult(self)) {
		error_set(DATABASE_ORACLE_ERROR_RESULT, "result is closed");
		return;
	}

//	PQclear(self->result);
	self->result = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
}

BOOL database_oracle_isResult(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return FALSE;

	return self->result != NULL;
}

BOOL database_oracle_nextResult(DATABASE_ORACLE *self) {
	if (database_oracle_isResult(self) && self->resultCur < (self->resultRow) - 1) {
		++(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_oracle_previousResult(DATABASE_ORACLE *self) {
	if (database_oracle_isResult(self) && self->resultCur > 0) {
		--(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_oracle_firstResult(DATABASE_ORACLE *self) {
	if (database_oracle_isResult(self)) {
		self->resultCur = 0;

		return TRUE;
	} else return FALSE;
}

BOOL database_oracle_lastResult(DATABASE_ORACLE *self) {
	if (database_oracle_isResult(self)) {
		self->resultCur = self->resultRow - 1;

		return TRUE;
	} else return FALSE;
}

int database_oracle_getResultRowCount(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return 0;

	return self->resultRow;
}

int database_oracle_getResultColumnCount(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return 0;

	return self->resultCol;
}

int database_oracle_getResultCursor(DATABASE_ORACLE *self) {
	if (!database_oracle_checkSelf(self)) return 0;

	return self->resultCur;
}

char *database_oracle_getString(DATABASE_ORACLE *self, int col) {
	if (!database_oracle_isResult(self)) return NULL;
	if (self->resultCur < 0 || self->resultCur >= self->resultRow) {
		error_set(DATABASE_ORACLE_ERROR_PARAMETER, "row out of range");
		return NULL;
	}
	if (col < 0 || col >= self->resultCol) {
		error_set(DATABASE_ORACLE_ERROR_PARAMETER, "column out of range");
		return NULL;
	}
	/*
	if (PQbinaryTuples(self->result)) {
		error_set(DATABASE_ORACLE_ERROR_FORMAT, "data format");
		return NULL;
	}
	
	if (PQgetisnull(self->result, self->resultCur, col)) return NULL;
	char *dbres = PQgetvalue(self->result, self->resultCur, col);
	char *res = (char *)malloc(strlen(dbres) + 1);
	if(res == NULL) {
		error_set(DATABASE_ORACLE_ERROR_MEMORY, "out of memory");
		return NULL;
	}
	strcpy(res, dbres);
	return res;
	*/

	return NULL;
}

void database_oracle_execute(DATABASE_ORACLE *self, const char *query, ...) {
	if (!database_oracle_checkSelf(self)) return;
		
	/* reset */
	if (database_oracle_isResult(self)) database_oracle_closeResult(self);

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
			char *esc = database_oracle_escapeParameter(val);
			stringbuffer_append(sb, esc);
			free(esc);
		} else {
			int val = va_arg(args, int);
			char esc[DATABASE_ORACLE_NUMBER_SIZE + 1];
			sprintf(esc, "%d", val);
			stringbuffer_append(sb, esc);
		}

        idx = i;                                /* set idx-start to next index */
	}
	stringbuffer_append(sb, text + idx);

	va_end(args);

//	self->result = PQexec(self->connection, stringbuffer_text(sb));
	stringbuffer_free(sb);
	free(text);
	if (self->result == NULL) {
		error_set(DATABASE_ORACLE_ERROR_EXECUTE, "unable to execute");
		return;
	}
/*
	switch(PQresultStatus(self->result)) {
		case PGRES_COMMAND_OK:
			return;
		case PGRES_TUPLES_OK:
			self->resultRow = PQntuples(self->result);
			self->resultCol = PQnfields(self->result);
			return;
		default:
			error_set(DATABASE_ORACLE_ERROR_EXECUTE, PQresultErrorMessage(self->result));
	}	
*/
}

char *database_oracle_getVersion(DATABASE_ORACLE *self) {
	database_oracle_execute(self, SQL_VERSION);

	if (!database_oracle_isResult(self)) return NULL;
	if (!database_oracle_nextResult(self)) return NULL;
	return database_oracle_getString(self, 0);
}
