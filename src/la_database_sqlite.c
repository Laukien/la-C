#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "la_file.h"
#include "la_message.h"
#include "la_database.h"

extern void database_throwError(DATABASE *self, int id, const char *message, const char *cause, const char *action);

void _database_new(DATABASE *self) {
	self->connection = NULL;
	self->result = NULL;
}

void _database_free(DATABASE *self) {
	self->result = NULL;
}

void _database_open(DATABASE *self) {
	int rc = sqlite3_open(self->name, &(self->connection));
	if (self->connection == NULL || rc) {
		database_throwError(self, DATABASE_ERROR_CONNECTION, "unable to init database", sqlite3_errmsg(self->connection), "check the the MYSQL configuration");
		return;
	}
}

void _database_close(DATABASE *self) {
	sqlite3_close(self->connection);
	self->connection = NULL;
}

BOOL _database_isOpen(DATABASE *self) {
	return self->connection != NULL;
}

void _database_closeResult(DATABASE *self) {
	/* free each pointer */
	int i;
	for (i = 0; i < self->resultRow * self->resultCol; ++i) {
		if (self->result[i * sizeof(char *)]) {
			free(self->result[i * sizeof(char *)]);
			self->result[i * sizeof(char *)] = NULL;
		}
	}
	free(self->result);
	self->result = NULL;
}

BOOL _database_isResult(DATABASE *self) {
	return self->result != NULL;
}

char *_database_getString(DATABASE *self, int col) {
	char *res = (self->result)[(self->resultRow * self->resultCur + col) * sizeof(char *)];
	if (res) {
		return strdup(res);
	}

	return NULL;
}

void _database_execute(DATABASE *self, const char *query) {
	self->result = (char **)malloc(1);           /* init result */
	self->resultRow = 0;
	self->resultCol = 0;
	sqlite3_stmt *stmt;

	int rc = sqlite3_prepare_v2(self->connection, query, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		database_throwError(self, DATABASE_ERROR_EXECUTE, "unable to compile statement", sqlite3_errmsg(self->connection), "check the query");
		return;
	}

	while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
		switch (rc) {
			case SQLITE_ERROR:
				database_throwError(self, DATABASE_ERROR_EXECUTE, "unable to get result", sqlite3_errmsg(self->connection), "check database file");
				sqlite3_finalize(stmt);
				return;
			case SQLITE_BUSY:
				sleep(1);                       /* wait */
				break;
			case SQLITE_ROW:
				self->resultRow++;
				if (!self->resultCol) {
					self->resultCol = sqlite3_column_count(stmt);
				}
			
				/* resize result */
				self->result = (char **)realloc(self->result, self->resultRow * self->resultCol * sizeof(char *));

				int i;
				char *text;
				for (i = 0; i < self->resultCol; ++i) {
					text = strdup((char *)sqlite3_column_text(stmt, i));
					self->result[(self->resultRow * (self->resultCol - 1) + i) * sizeof(char *)] = text;
				}
				break;
			default:
				database_throwError(self, DATABASE_ERROR_EXECUTE, "unable to get result", sqlite3_errmsg(self->connection), "check internal logic");
		}
	}

	sqlite3_finalize(stmt);
	
}

BOOL _database_checkParameter(DATABASE *self) {
	return TRUE;
}

char *_database_escapeString(DATABASE *self, const char *query) {
	char *res = sqlite3_mprintf(query);
	
	return res;
}
