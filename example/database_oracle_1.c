#include "la_database_oracle.h"
#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>

char *username = "lichtenstein_dwh";
char *password = "lichtenstein_dwh";
char *sid = "dwh";
char *sql = "SELECT * FROM d_betrieb WHERE id>:x";

int main(void) {

	printf ( "NEW\n" );
	DATABASE_ORACLE *db = database_oracle_new();
	if (error_exists()) {
		error_show();
		exit (1);
	}

	if (db == NULL) printf ( "DB -> NULL\n" );

	printf ( "FREE\n" );
	database_oracle_free(db);

	static OCIEnv *p_env;
	static OCIError *p_err;
	static OCISvcCtx *p_svc;
	static OCIStmt *p_sql;
	static OCIDefine *p_dfn = (OCIDefine *) 0;
	static OCIBind *p_bnd = (OCIBind *) 0;

	int p_bvi;
	char p_sli[20];
	int rc;
	OraText errbuf[100];
	int errcode;

	/* Initialize OCI */
	printf ( "Initialize OCI\n" );
	rc = OCIInitialize((ub4) OCI_DEFAULT, (dvoid *)0,
		(dvoid * (*)(dvoid *, size_t)) 0,
		(dvoid * (*)(dvoid *, dvoid *, size_t))0,
		(void (*)(dvoid *, dvoid *)) 0 );
	
	/* Initialize evironment */
	printf ( "Initialize evironment\n" );
	rc = OCIEnvInit( (OCIEnv **) &p_env, OCI_DEFAULT, (size_t) 0, (dvoid **) 0 );

	/* Initialize handles */
	printf ( "Initialize handles\n" );
	rc = OCIHandleAlloc( (dvoid *) p_env, (dvoid **) &p_err, OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0);
	rc = OCIHandleAlloc( (dvoid *) p_env, (dvoid **) &p_svc, OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0);

	/* Connect to database server */
	printf ( "Connect to database server\n" );
	rc = OCILogon(p_env, p_err, &p_svc, (const OraText *)username, strlen(username), (const OraText *)password, strlen(password), (const OraText *)sid, strlen(sid));
	if (rc != 0) {
		OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
		printf("Error - %.*s\n", 512, errbuf);
		exit(1);
	}

	/* Allocate and prepare SQL statement */
	printf ( "Allocate and prepare SQL statement\n" );
	rc = OCIHandleAlloc( (dvoid *) p_env, (dvoid **) &p_sql, OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0);
	rc = OCIStmtPrepare(p_sql, p_err, (const OraText *)sql, (ub4) strlen(sql), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);

	/* Bind the values for the bind variables */
	printf ( "Bind the values for the bind variables\n" );
	p_bvi = 10;                                 /* Use DEPTNO=10 */
	rc = OCIBindByName(p_sql, &p_bnd, p_err, (text *) ":x", -1, (dvoid *) &p_bvi, sizeof(int), SQLT_INT, (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT);

	/* Define the select list items */
	printf ( "Define the select list items\n" );
	rc = OCIDefineByPos(p_sql, &p_dfn, p_err, 1, (dvoid *) &p_sli, (sword) 20, SQLT_STR, (dvoid *) 0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT);

	/* Execute the SQL statment */
	printf ( "Execute the SQL statment\n" );
	rc = OCIStmtExecute(p_svc, p_sql, p_err, (ub4) 1, (ub4) 0, (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT);

	/* Fetch the remaining data */
	printf ( "Fetch the remaining data\n" );
	while (rc != OCI_NO_DATA) {
		printf("%s\n",p_sli);
		rc = OCIStmtFetch(p_sql, p_err, 1, 0, 0);
	}

	/* Disconnect */
	printf ( "Disconnect\n" );
	rc = OCILogoff(p_svc, p_err);

	/* Free handles */
	printf ( "Free handles\n" );
	rc = OCIHandleFree((dvoid *) p_sql, OCI_HTYPE_STMT);
	rc = OCIHandleFree((dvoid *) p_svc, OCI_HTYPE_SVCCTX);
	rc = OCIHandleFree((dvoid *) p_err, OCI_HTYPE_ERROR);

	return 0;
}
