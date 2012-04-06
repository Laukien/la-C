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
#include "la_number.h"
#include "la_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

BOOL file_exists(const char *filename) {
	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return FALSE;

	fclose(pFile);
	return TRUE;
}

char *file_name(const char *file) {
	int i;
	for (i = strlen(file)-1; i >= -1; i--) {
		if (i == -1) break;                     /* not found */
		if (file[i] == DIRECTORY_SEPARATOR_CHAR) break;
	}

	int l = strlen(file) - i;                   /* len + 1 */
	char *name = (char *) malloc(l);
	if(name == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(name, file + i + 1, l);              /* name + '\0' */

	return name;
}

char *file_temp() {
	time_t t;
	time(&t);
	srand((unsigned int) t);

	int pid = getpid();
	char *pid_s = number_intToString(pid);
	int rnd = rand();
	char *rnd_s = number_intToString(rnd);

	char *dir = directory_temp();	
	size_t size = strlen(dir) + 1 + strlen(pid_s) + 1 + strlen(rnd_s);
	dir = (char *) realloc(dir, size + 1);
	strcat(dir, DIRECTORY_SEPARATOR_STRING);
	strcat(dir, pid_s);
	strcat(dir, "_");
	strcat(dir, rnd_s);

	return dir;
}

#ifdef __cplusplus
namespace file {
	bool exists(const std::string filename) {
		return file_exists(filename.c_str());
	}

	std::string name(const std::string filename) {
		char *name = file_name(filename.c_str());	
		std::string names = name;
		free(name);

		return names;
	}

	std::string temp() {
		char *file = file_temp();
		std::string tmp = file;
		free(file);

		return tmp;
	}
}
#endif
