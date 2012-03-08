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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int file_exists(char *filename) {
	assert(filename == NULL);

	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return 0;

	fclose(pFile);
	return 1;
}
