#include "la_database_oracle.h"
#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

	printf ( "NEW\n" );
	DATABASE_ORACLE *db = database_oracle_new();
	if (error_exists()) {
		error_show();
		exit (1);
	}

	if (db == NULL) printf ( "DB -> NULL\n" );
	return 0;
}
