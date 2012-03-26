/*
 * =====================================================================================
 *
 *       Filename:  la_file.c
 *
 *    Description:  Input-/Output...file-operations
 *
 *        Version:  1.0
 *        Created:  11/11/2011 02:25:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "la_file.h"
#include "la_directory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BOOL file_exists(char *filename) {
	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return FALSE;

	fclose(pFile);
	return TRUE;
}

char *file_name(char *file) {
	int i;
	for (i = strlen(file)-1; i >= -1; i--) {
		if (i == -1) break;                     /* not found */
		if (file[i] == DIRECTORY_SEPARATOR) break;
	}

	int l = strlen(file) - i;                   /* len + 1 */
	char *name = malloc(l);
	if(name == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(name, file + i + 1, l);              /* name + '\0' */

	return name;
}
