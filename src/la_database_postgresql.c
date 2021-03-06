/*
 * =====================================================================================
 *
 *       Filename:  la_database_postgresql.c
 *    Description:  PostgreSQL wrapper
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "la_message.h"
#include "la_database.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

extern void database_throwError(DATABASE *self, int id, const char *message, const char *cause, const char *action);

void _database_new(DATABASE *self) {
	self->host = NULL;
	self->port = DATABASE_PORT;
	self->user = NULL;
	self->password = NULL;
	self->schema = NULL;
	self->connection = NULL;
	self->result = NULL;
}

void _database_free(DATABASE *self) {
	if (self->host) {
		free(self->host);
		self->host = NULL;
	}
	self->port = 0;
	if (self->user) {
		free(self->user);
		self->user = NULL;
	}
	if (self->password) {
		free(self->password);
		self->password = NULL;
	}
	if (self->schema) {
		free(self->schema);
		self->schema = NULL;
	}
}

void _database_open(DATABASE *self) {
	char conn[DATABASE_CONNECTION_SIZE + 1];
	snprintf(conn, DATABASE_CONNECTION_SIZE, "host=%s port=%d dbname=%s user=%s password=%s", self->host, self->port, self->name, self->user, self->password);
	self->connection = PQconnectdb(conn);
	if (PQstatus(self->connection) == CONNECTION_BAD) {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to open database", PQerrorMessage(self->connection), "check connection parameters");
	}
}

void _database_close(DATABASE *self) {
	PQfinish(self->connection);
	self->connection = NULL;
}

BOOL _database_isOpen(DATABASE *self) {
	return self->connection != NULL;
}

void _database_closeResult(DATABASE *self) {
	PQclear(self->result);
	self->result = NULL;
}

BOOL _database_isResult(DATABASE *self) {
	return self->result != NULL;
}

char *_database_getString(DATABASE *self, int col) {
	if (PQbinaryTuples(self->result)) {
		database_throwError(self, DATABASE_ERROR_FORMAT, "unable to get result", "the data is nopt a string", "check the the format");
		return NULL;
	}
	
	if (PQgetisnull(self->result, self->resultCur, col)) return NULL;
	char *dbres = PQgetvalue(self->result, self->resultCur, col);
	char *res = (char *)malloc(strlen(dbres) + 1);
	if(res == NULL) {
		database_throwError(self, DATABASE_ERROR_MEMORY, "unable to get memory", "out of memory", "get more memory or redesign your query");
		return NULL;
	}
	strcpy(res, dbres);

	return res;
}

void _database_execute(DATABASE *self, const char *query) {
	self->result = PQexec(self->connection, query);
	if (self->result == NULL) {
		database_throwError(self, DATABASE_ERROR_EXECUTE, "no result", "unable to get result from database", "check your query");
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
			database_throwError(self, DATABASE_ERROR_EXECUTE, "invalid status", PQresultErrorMessage(self->result), "check the query");
	}	
}

BOOL _database_checkParameter(DATABASE *self) {
	if (
		self->host == NULL
		||
		self->port <= 0
		||
		self->user == NULL
		||
		self->password == NULL
	) {
		database_throwError(self, DATABASE_ERROR_PARAMETER, "wrong or un-set parameter", "invalid parameter", "check parameters");
		return FALSE;
	}

	return TRUE;
}

char *_database_escapeString(DATABASE *self, const char *query) {
	char *result = (char *)malloc(strlen(query) * 2 + 1);
	PQescapeString(result, query, strlen(query));
	return result;
}
