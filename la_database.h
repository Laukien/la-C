#ifndef LA_DATABASE_H
#define LA_DATABASE_H

#if defined LA_DATABASE_POSTGRESQL_H
		#include <libpq-fe.h>
#elif defined LA_DATABASE_MYSQL_H
//	#include <my_global.h>
	#include <mysql.h>
#endif

#include "la_common.h"
#include "la_error.h"

#define LA_DATABASE_ERROR_INIT 1
#define LA_DATABASE_ERROR_MEMORY 2
#define LA_DATABASE_ERROR_PARAMETER 3
#define LA_DATABASE_ERROR_CONNECTION 4
#define LA_DATABASE_ERROR_EXECUTE 5
#define LA_DATABASE_ERROR_RESULT 6
#define LA_DATABASE_ERROR_FORMAT 7

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *host;
	char *name;
	char *user;
	char *password;
	int resultCol;
	int resultRow;
	int resultCur;
#if defined LA_DATABASE_POSTGRESQL_H
	char *schema;
	PGconn *connection;
	PGresult *result;
#elif defined LA_DATABASE_MYSQL_H
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
#elif defined LA_DATABASE_ORACLE_H
	char *schema;
	char *sid;
#endif
} LA_DATABASE;

LA_DATABASE *database_new();
void database_free(LA_DATABASE *self);
void database_open(LA_DATABASE *self);
void database_close(LA_DATABASE *self);
BOOL database_isOpen(LA_DATABASE *self);
void database_closeResult(LA_DATABASE *self);
BOOL database_isResult(LA_DATABASE *self);
int database_getResultRowCount(LA_DATABASE *self);
int database_getResultColumnCount(LA_DATABASE *self);
int database_getResultCursor(LA_DATABASE *self);
BOOL database_nextResult(LA_DATABASE *self);
BOOL database_previousResult(LA_DATABASE *self);
BOOL database_firstResult(LA_DATABASE *self);
BOOL database_lastResult(LA_DATABASE *self);
void database_setHost(LA_DATABASE *self, const char *host);
void database_setName(LA_DATABASE *self, const char *name);
void database_setUser(LA_DATABASE *self, const char *user);
void database_setPassword(LA_DATABASE *self, const char *password);
char *database_getString(LA_DATABASE *self, int col);
void database_execute(LA_DATABASE *self, const char *query, ...);
char *database_getVersion(LA_DATABASE *self);
BOOL database_checkSelf(LA_DATABASE *self);

#ifdef __cplusplus
}
#endif

#endif
