#ifndef LA_DATABASE_POSTGRESQL_H
#define LA_DATABASE_POSTGRESQL_H

#include <libpq-fe.h>
#include "la_common.h"
#include "la_error.h"
#include "la_exception.h"

#define DATABASE_POSTGRESQL_ERROR_INIT 1
#define DATABASE_POSTGRESQL_ERROR_MEMORY 2
#define DATABASE_POSTGRESQL_ERROR_PARAMETER 3
#define DATABASE_POSTGRESQL_ERROR_CONNECTION 4
#define DATABASE_POSTGRESQL_ERROR_EXECUTE 5
#define DATABASE_POSTGRESQL_ERROR_RESULT 6
#define DATABASE_POSTGRESQL_ERROR_FORMAT 7

#define DATABASE_POSTGRESQL_CONNECTION_SIZE 512
#define DATABASE_POSTGRESQL_QUERY_SIZE 4096
#define DATABASE_POSTGRESQL_NUMBER_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
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
	char error[ERROR_MESSAGE_SIZE + 1];
} DATABASE_POSTGRESQL;

DATABASE_POSTGRESQL *database_postgresql_new();
void database_postgresql_setException(DATABASE_POSTGRESQL *self, EXCEPTION *e);
void database_postgresql_free(DATABASE_POSTGRESQL *self);
void database_postgresql_open(DATABASE_POSTGRESQL *self);
void database_postgresql_close(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_isOpen(DATABASE_POSTGRESQL *self);
void database_postgresql_closeResult(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_isResult(DATABASE_POSTGRESQL *self);
int database_postgresql_getResultRowCount(DATABASE_POSTGRESQL *self);
int database_postgresql_getResultColumnCount(DATABASE_POSTGRESQL *self);
int database_postgresql_getResultCursor(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_nextResult(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_previousResult(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_firstResult(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_lastResult(DATABASE_POSTGRESQL *self);
void database_postgresql_setHost(DATABASE_POSTGRESQL *self, const char *host);
void database_postgresql_setPort(DATABASE_POSTGRESQL *self, int port);
void database_postgresql_setName(DATABASE_POSTGRESQL *self, const char *name);
void database_postgresql_setUser(DATABASE_POSTGRESQL *self, const char *user);
void database_postgresql_setPassword(DATABASE_POSTGRESQL *self, const char *password);
char *database_postgresql_getString(DATABASE_POSTGRESQL *self, int col);
void database_postgresql_execute(DATABASE_POSTGRESQL *self, const char *query, ...);
char *database_postgresql_getVersion(DATABASE_POSTGRESQL *self);
BOOL database_postgresql_checkSelf(DATABASE_POSTGRESQL *self);
void database_postgresql_setSchema(DATABASE_POSTGRESQL *self, const char *schema);
BOOL isError(DATABASE_POSTGRESQL *self);
char *getError(DATABASE_POSTGRESQL *self);

#ifdef __cplusplus
}
#endif

#endif
