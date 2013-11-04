#ifndef LA_DATABASE_H
#define LA_DATABASE_H

#include "la_boolean.h"
#include "la_error.h"
#include "la_exception.h"

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

#if defined DATABASE_POSTGRESQL
#include <libpq-fe.h>
#define DATABASE_PORT 5432
#define DATABASE_SQL_NEXTID "SELECT nextval('system.session_id_seq'::regclass)"
#define DATABASE_SQL_RANDOM "SELECT (random()*65535)::int"
#define DATABASE_SQL_VERSION "SELECT version()"
#elif defined DATABASE_MYSQL
#include <mysql.h>
#elif defined DATABASE_ORACLE
#define SQL_VERSION "SELECT * FROM v$version;"
#else
#define DATABASE_PORT -1
#define DATABASE_SQL_NEXTID ""
#define DATABASE_SQL_RANDOM ""
#define DATABASE_SQL_VERSION ""
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct la_database {
	ERROR *error;
	EXCEPTION *exception;
	char *host;
	int port;
	char *name;
	char *user;
	char *password;
	int resultCol;
	int resultRow;
	int resultCur;
#if defined DATABASE_POSTGRESQL
	char *schema;
	PGconn *connection;
	PGresult *result;
#elif defined DATABASE_MYSQL
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
#elif defined DATABASE_ORACLE
	char *schema;
	char *sid;
#endif
};
typedef struct la_database DATABASE;

DATABASE *database_new();
void database_setError(DATABASE *self, ERROR *e);
void database_setException(DATABASE *self, EXCEPTION *e);
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
char *database_getString(DATABASE *self, int col);
void database_execute(DATABASE *self, const char *query, ...);
char *database_getVersion(DATABASE *self);
BOOL database_checkSelf(DATABASE *self);
void database_setSchema(DATABASE *self, const char *schema);
BOOL isError(DATABASE *self);
char *getError(DATABASE *self);
void database_setHost(DATABASE *self, const char *host);
void database_setPort(DATABASE *self, int port);
void database_setName(DATABASE *self, const char *name);
void database_setUser(DATABASE *self, const char *user);
void database_setPassword(DATABASE *self, const char *password);
#if defined DATABASE_POSTGRESQL
void database_setSchema(DATABASE *self, const char *schema);
#endif

#ifdef __cplusplus
}
#endif

#endif
