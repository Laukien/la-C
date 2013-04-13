#ifndef LA_DATABASE_ORACLE_H
#define LA_DATABASE_ORACLE_H

#include <libpq-fe.h>
#include "la_common.h"
#include "la_error.h"

#define DATABASE_ORACLE_ERROR_INIT 1
#define DATABASE_ORACLE_ERROR_MEMORY 2
#define DATABASE_ORACLE_ERROR_PARAMETER 3
#define DATABASE_ORACLE_ERROR_CONNECTION 4
#define DATABASE_ORACLE_ERROR_EXECUTE 5
#define DATABASE_ORACLE_ERROR_RESULT 6
#define DATABASE_ORACLE_ERROR_FORMAT 7

#define DATABASE_ORACLE_CONNECTION_SIZE 512
#define DATABASE_ORACLE_QUERY_SIZE 4096
#define DATABASE_ORACLE_NUMBER_SIZE 64

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *host;
	int port;
	char *name;
	char *user;
	char *password;
	char *schema;
	char *sid;
	int resultCol;
	int resultRow;
	int resultCur;
	PGconn *connection;
	PGresult *result;
} DATABASE_ORACLE;

DATABASE_ORACLE *database_oracle_new();
void database_oracle_free(DATABASE_ORACLE *self);
void database_oracle_open(DATABASE_ORACLE *self);
void database_oracle_close(DATABASE_ORACLE *self);
BOOL database_oracle_isOpen(DATABASE_ORACLE *self);
void database_oracle_closeResult(DATABASE_ORACLE *self);
BOOL database_oracle_isResult(DATABASE_ORACLE *self);
int database_oracle_getResultRowCount(DATABASE_ORACLE *self);
int database_oracle_getResultColumnCount(DATABASE_ORACLE *self);
int database_oracle_getResultCursor(DATABASE_ORACLE *self);
BOOL database_oracle_nextResult(DATABASE_ORACLE *self);
BOOL database_oracle_previousResult(DATABASE_ORACLE *self);
BOOL database_oracle_firstResult(DATABASE_ORACLE *self);
BOOL database_oracle_lastResult(DATABASE_ORACLE *self);
void database_oracle_setHost(DATABASE_ORACLE *self, const char *host);
void database_oracle_setName(DATABASE_ORACLE *self, const char *name);
void database_oracle_setUser(DATABASE_ORACLE *self, const char *user);
void database_oracle_setPassword(DATABASE_ORACLE *self, const char *password);
char *database_oracle_getString(DATABASE_ORACLE *self, int col);
void database_oracle_execute(DATABASE_ORACLE *self, const char *query, ...);
char *database_oracle_getVersion(DATABASE_ORACLE *self);
BOOL database_oracle_checkSelf(DATABASE_ORACLE *self);
void database_oracle_setSchema(DATABASE_ORACLE *self, const char *schema);

#ifdef __cplusplus
}
#endif

#endif
