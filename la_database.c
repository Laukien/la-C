#include "la_database.h"
#include <stdlib.h>
#include <string.h>

BOOL database_checkSelf(LA_DATABASE *self) {
	error_reset();
	if (self == NULL) {
		error_set(LA_DATABASE_ERROR_INIT, "database not initiated");
		return FALSE;
	}

	return TRUE;
}

void database_setHost(LA_DATABASE *self, const char *host) {
	if (!database_checkSelf(self)) return;

	self->host = (char *)malloc(strlen(host) + 1);
	strcpy(self->host, host);
}

void database_setName(LA_DATABASE *self, const char *name) {
	if (!database_checkSelf(self)) return;

	self->name = (char *)malloc(strlen(name) + 1);
	strcpy(self->name, name);
}

void database_setUser(LA_DATABASE *self, const char *user) {
	if (!database_checkSelf(self)) return;

	self->user = (char *)malloc(strlen(user) + 1);
	strcpy(self->user, user);
}

void database_setPassword(LA_DATABASE *self, const char *password) {
	if (!database_checkSelf(self)) return;

	self->password = (char *)malloc(strlen(password) + 1);
	strcpy(self->password, password);
}

int database_getResultRowCount(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return 0;

	return self->resultRow;
}

int database_getResultColumnCount(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return 0;

	return self->resultCol;
}

int database_getResultCursor(LA_DATABASE *self) {
	if (!database_checkSelf(self)) return 0;

	return self->resultCur;
}

