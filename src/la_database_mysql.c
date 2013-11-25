#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "la_error.h"
#include "la_message.h"
#include "la_database.h"

extern void database_throwError(DATABASE *self, int id, const char *message, const char *cause, const char *action);

void _database_new(DATABASE *self) {
	self->host = NULL;
	self->port = DATABASE_PORT;
	self->result = NULL;
	self->connection = NULL;
}

void _database_free(DATABASE *self) {
	if (self->host != NULL) free (self->host);
	self->port = -1;
}

void _database_open(DATABASE *self) {
	self->connection = mysql_init(NULL);
	if (self->connection == NULL || mysql_errno(self->connection)) {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to init database", mysql_error(self->connection), "check the the MYSQL configuration");
		return;
	}

	if (mysql_real_connect(self->connection, self->host, self->user, self->password, self->name, self->port, NULL, 0) == NULL) {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to open database", mysql_error(self->connection), "check connection parameters");
	}
}

void _database_close(DATABASE *self) {
	mysql_close(self->connection);
	self->connection = NULL;
}

BOOL _database_isOpen(DATABASE *self) {
	return self->connection != NULL;
}

void _database_closeResult(DATABASE *self) {
	mysql_free_result(self->result);
	self->result = NULL;
}

BOOL _database_isResult(DATABASE *self) {
	return self->result != NULL;
}

char *_database_getString(DATABASE *self, int col) {
	mysql_data_seek(self->result, self->resultCur);
	self->row = mysql_fetch_row(self->result);
	if (self->row == NULL)  {
		database_throwError(self, DATABASE_ERROR_RESULT, "unable to get result row", mysql_error(self->connection), "check the query");
		return NULL;
	}
	
	char *res = (char *)malloc(strlen(self->row[col]) + 1);
	if(res == NULL) {
		database_throwError(self, DATABASE_ERROR_MEMORY, "unable to get memory", "out of memory", "get more memory or redesign your query");
		return NULL;
	}
	strcpy(res, self->row[col]);

	return res;
}

void _database_execute(DATABASE *self, const char *query) {
	if (mysql_query(self->connection, query)) {
		database_throwError(self, DATABASE_ERROR_EXECUTE, "unable to parse query", mysql_error(self->connection), "check your query");
		return;
	}

	self->result = mysql_store_result(self->connection);
	if (!self->result) {
		self->resultRow = 0;
		self->resultCol = 0;
	} else {
		self->resultRow = mysql_num_rows(self->result);
		self->resultCol = mysql_num_fields(self->result);
	}
}

BOOL _database_checkParameter(DATABASE *self) {
	if (
		self->host == NULL
		||
		self->port <= 0
	) {
		database_throwError(self, DATABASE_ERROR_PARAMETER, "wrong or un-set parameter", "invalid parameter", "check parameters");
		return FALSE;
	}

	return TRUE;
}

char *_database_escapeString(DATABASE *self, const char *query) {
	char *res = (char *)malloc(strlen(query) * 2 + 1);
	mysql_real_escape_string(self->connection, res, query, strlen(query));
	
	return res;
}
