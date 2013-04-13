#include "la_database.h"
#include <stdlib.h>
#include <string.h>

BOOL database_checkSelf(DATABASE *self) {
	error_reset();
	if (self == NULL) {
		error_set(DATABASE_ERROR_INIT, "database not initiated");
		return FALSE;
	}

	return TRUE;
}

void database_setHost(DATABASE *self, const char *host) {
	if (!database_checkSelf(self)) return;

	self->host = (char *)malloc(strlen(host) + 1);
	strcpy(self->host, host);
}

void database_setName(DATABASE *self, const char *name) {
	if (!database_checkSelf(self)) return;

	self->name = (char *)malloc(strlen(name) + 1);
	strcpy(self->name, name);
}

void database_setUser(DATABASE *self, const char *user) {
	if (!database_checkSelf(self)) return;

	self->user = (char *)malloc(strlen(user) + 1);
	strcpy(self->user, user);
}

void database_setPassword(DATABASE *self, const char *password) {
	if (!database_checkSelf(self)) return;

	self->password = (char *)malloc(strlen(password) + 1);
	strcpy(self->password, password);
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
	if (!database_checkSelf(self)) return 0;

	return self->resultRow;
}

int database_getResultColumnCount(DATABASE *self) {
	if (!database_checkSelf(self)) return 0;

	return self->resultCol;
}

int database_getResultCursor(DATABASE *self) {
	if (!database_checkSelf(self)) return 0;

	return self->resultCur;
}

