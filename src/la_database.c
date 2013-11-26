#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "la_message.h"
#include "la_number.h"
#include "la_stringbuffer.h"
#include "la_database.h"

extern void _database_new(DATABASE *self);
extern void _database_free(DATABASE *self);
extern void _database_open(DATABASE *self);
extern void _database_close(DATABASE *self);
extern BOOL _database_isOpen(DATABASE *self);
extern void _database_closeResult(DATABASE *self);
extern BOOL _database_isResult(DATABASE *self);
extern char *_database_getString(DATABASE *self, int col);
extern void _database_execute(DATABASE *self, const char *query);
extern BOOL _database_checkParameter(DATABASE *self);
extern char *_database_escapeString(DATABASE *self, const char *query);

DATABASE *database_new() {
	DATABASE *self = (DATABASE *)malloc(sizeof(DATABASE));
	if (!self) {
		message_error("unable to get memory");
	}

	self->exception = NULL;
	self->name = NULL;
	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
	
	_database_new(self);

	return self;
}

void database_free(DATABASE *self) {
	assert(self != NULL);

	/* close objects */
	if (database_isResult(self)) database_closeResult(self);
	if (database_isOpen(self)) database_close(self);

	/* free strings */
	if (self->name)  {
		free(self->name);
		self->name = NULL;
	}
	
	_database_free(self);

	/* free database */
	free(self);
	self = NULL;
}

void database_throwError(DATABASE *self, int id, const char *message, const char *cause, const char *action) {
		assert(self);

		if (self->exception) {
			exception_setLong(self->exception, id, message, cause, action);
			exception_setComment(self->exception, "database");
			exception_throw(self->exception);
		}
}

void database_setException(DATABASE *self, EXCEPTION *e) {
	assert(self != NULL);
	assert(e != NULL);

	self->exception = e;
}

BOOL database_checkParameter(DATABASE *self) {
	if (
		self->name == NULL
		||
		!_database_checkParameter(self)
	) {
		database_throwError(self, DATABASE_ERROR_PARAMETER, "wrong or un-set parameter", "invalid parameter", "check parameters");
		return FALSE;
	}

	return TRUE;
}
void database_open(DATABASE *self) {
	assert(self != NULL);
	if (!database_checkParameter(self)) return;

	if (database_isOpen(self)) {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to open connection", "connection already opened", "check for an other \"open\"");
		return;
	}

	_database_open(self);
}

void database_close(DATABASE *self) {
	assert(self != NULL);

	if (database_isOpen(self)) {
		if (database_isResult(self)) database_closeResult(self);
		_database_close(self);
	} else {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to close connection", "connection already closed", "check for an other \"close\"");
	}
}

BOOL database_isOpen(DATABASE *self) {
	assert(self != NULL);

	return _database_isOpen(self);
}

void database_closeResult(DATABASE *self) {
	assert(self != NULL);

	if (!database_isResult(self)) {
		database_throwError(self, DATABASE_ERROR_RESULT, "unable to close result", "result has been closed already", "check for an other \"close\"");
		return;
	}

	_database_closeResult(self);

	self->resultCol = 0;
	self->resultRow = 0;
	self->resultCur = -1;
}

BOOL database_isResult(DATABASE *self) {
	assert(self != NULL);

	return _database_isResult(self);
}

BOOL database_nextResult(DATABASE *self) {
	if (database_isResult(self) && self->resultCur < (self->resultRow) - 1) {
		++(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_previousResult(DATABASE *self) {
	if (database_isResult(self) && self->resultCur > 0) {
		--(self->resultCur);

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL database_firstResult(DATABASE *self) {
	if (database_isResult(self)) {
		self->resultCur = 0;

		return TRUE;
	} else return FALSE;
}

BOOL database_lastResult(DATABASE *self) {
	if (database_isResult(self)) {
		self->resultCur = self->resultRow - 1;

		return TRUE;
	} else return FALSE;
}

int database_getResultRowCount(DATABASE *self) {
	assert(self != NULL);

	return self->resultRow;
}

int database_getResultColumnCount(DATABASE *self) {
	assert(self != NULL);

	return self->resultCol;
}

int database_getResultCursor(DATABASE *self) {
	assert(self != NULL);

	return self->resultCur;
}

char *database_getString(DATABASE *self, int col) {
	assert(self);

	if (!database_isResult(self)) {
		database_throwError(self, DATABASE_ERROR_RESULT, "unable to get result", "no result has been set before", "check your query or my you don't asked for a result");
		return NULL;
	}
	if (self->resultCur < 0 || self->resultCur >= self->resultRow) {
		database_throwError(self, DATABASE_ERROR_PARAMETER, "unable to get result", "row out of range", "check the row count");
		return NULL;
	}
	if (col < 0 || col >= self->resultCol) {
		database_throwError(self, DATABASE_ERROR_PARAMETER, "unable to get result", "column out of range", "check for column count");
		return NULL;
	}

	return _database_getString(self, col);
}

int database_getNumber(DATABASE *self, int col) {
	assert(self);

	char *str = database_getString(self, col);
	if (!str) {
		return -1;
	}

	int num;
	if (number_isNumber(str)) {
		num = number_toInteger(str);
	} else {
		num = -1;
	}
	free(str);

	return num;
}

void database_execute(DATABASE *self, const char *query, ...) {
	assert(self != NULL);
		
	/* reset */
	if (database_isResult(self)) database_closeResult(self);

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
			char *esc = _database_escapeString(self, val);
			stringbuffer_append(sb, esc);
			free(esc);
		} else {
			int val = va_arg(args, int);
			char esc[DATABASE_NUMBER_SIZE + 1];
			sprintf(esc, "%d", val);
			stringbuffer_append(sb, esc);
		}

        idx = i;                                /* set idx-start to next index */
	}
	stringbuffer_append(sb, text + idx);

	va_end(args);
	free(text);

	_database_execute(self, stringbuffer_getTextPointer(sb));
	stringbuffer_free(sb);
}

char *database_getVersion(DATABASE *self) {
	database_execute(self, DATABASE_SQL_VERSION);

	if (!database_isResult(self)) return NULL;
	if (!database_nextResult(self)) return NULL;
	return database_getString(self, 0);
}

int database_getRandom(DATABASE *self) {
	database_execute(self, DATABASE_SQL_RANDOM);

	if (!database_isResult(self)) return -1;
	if (!database_nextResult(self)) return -1;
	return database_getNumber(self, 0);
}

void database_setName(DATABASE *self, const char *name) {
	assert(self != NULL);
	assert(name != NULL);

	self->name = strdup(name);
}

#if defined(DATABASE_MYSQL) || defined(DATABASE_ORACLE) || defined(DATABASE_POSTGRESQL)
void database_setHost(DATABASE *self, const char *host) {
	assert(self != NULL);
	assert(host != NULL);

	self->host = strdup(host);
}

void database_setPort(DATABASE *self, int port) {
	assert(self != NULL);
	assert(port > 0);

	self->port = port;
}

void database_setUser(DATABASE *self, const char *user) {
	assert(self != NULL);
	assert(user != NULL);

	self->user = strdup(user);
}

void database_setPassword(DATABASE *self, const char *password) {
	assert(self != NULL);
	assert(password != NULL);

	self->password = strdup(password);
}
#endif

#if defined(DATABASE_ORACLE) || defined(DATABASE_POSTGRESQL)
void database_setSchema(DATABASE *self, const char *schema) {
	assert(self != NULL);
	assert(schema != NULL);

	self->schema = strdup(schema);
}
#endif
