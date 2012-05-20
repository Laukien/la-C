#ifndef LA_DATABASE_H
#define LA_DATABASE_H

#if defined DATABASE_POSTGRESQL_H
		#include <libpq-fe.h>
#elif defined DATABASE_MYSQL_H
//	#include <my_global.h>
	#include <mysql.h>
#endif

#include "la_common.h"
#include "la_error.h"

#define DATABASE_ERROR_INIT 1
#define DATABASE_ERROR_MEMORY 2
#define DATABASE_ERROR_PARAMETER 3
#define DATABASE_ERROR_CONNECTION 4
#define DATABASE_ERROR_EXECUTE 5
#define DATABASE_ERROR_RESULT 6
#define DATABASE_ERROR_FORMAT 7

#define DATABASE_CONNECTION_SIZE 512
#define DATABASE_QUERY_SIZE 4096
#define DATABASE_NUMBER_SIZE 64

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
#if defined DATABASE_POSTGRESQL_H
	char *schema;
	PGconn *connection;
	PGresult *result;
#elif defined DATABASE_MYSQL_H
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
#elif defined DATABASE_ORACLE_H
	char *schema;
	char *sid;
#endif
} DATABASE;

DATABASE *database_new();
void database_free(DATABASE *self);
void database_open(DATABASE *self);
void database_close(DATABASE *self);
BOOL database_isOpen(DATABASE *self);
void database_closeResult(DATABASE *self);
BOOL database_isResult(DATABASE *self);
int database_getResultRowCount(DATABASE *self);
int database_getResultColumnCount(DATABASE *self);
int database_getResultCursor(DATABASE *self);
BOOL database_nextResult(DATABASE *self);
BOOL database_previousResult(DATABASE *self);
BOOL database_firstResult(DATABASE *self);
BOOL database_lastResult(DATABASE *self);
void database_setHost(DATABASE *self, const char *host);
void database_setName(DATABASE *self, const char *name);
void database_setUser(DATABASE *self, const char *user);
void database_setPassword(DATABASE *self, const char *password);
char *database_getString(DATABASE *self, int col);
void database_execute(DATABASE *self, const char *query, ...);
char *database_getVersion(DATABASE *self);
BOOL database_checkSelf(DATABASE *self);

#ifdef __cplusplus
}
#endif

#endif
