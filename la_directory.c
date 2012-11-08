/*
 * =====================================================================================
 *
 *       Filename:  la_directory.c
 *
 *    Description:  Input-/Output...directory-operations
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

#include "la_directory.h"
#include "la_system.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef SYSTEM_OS_TYPE_WINDOWS
#include <dirent.h>
#endif

BOOL directory_create(const char *directoryname) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	return (mkdir(directoryname)) == 0 ? TRUE : FALSE;
#else
	return (mkdir(directoryname, 0755)) == 0 ? TRUE : FALSE;
#endif
}

BOOL directory_exists(const char *name) {
	struct stat st;
	return (stat(name, &st) == 0);
}

char *directory_temp() {
	char *dir;
#ifdef SYSTEM_OS_TYPE_UNIX
	dir = (char *) malloc(5);
	strcpy(dir, "/tmp");
#else
	char *tmp = getenv("TEMP");
	if (tmp != NULL) {
		dir = (char *) malloc(strlen(tmp) + 1);
		strcpy(dir, tmp);
	} else {
		char *win = getenv("WINDIR");
		if (win == NULL) {
			fprintf(stderr, "ERROR: environment variable 'windir' not set\n");
			exit(EXIT_FAILURE);
		}
		dir = (char *) malloc(strlen(win) + 6);
		strcpy(dir, win);
		strcat(dir, DIRECTORY_SEPARATOR_STRING);
		strcat(dir, "Temp");
	}
#endif

	return dir;
}

#ifdef __cplusplus
namespace directory {
	bool create(const std::string name) {
		return directory_create(name.c_str());
	}

	bool exists(const std::string name) {
		return directory_exists(name.c_str());
	}

	std::string temp() {
		char *dir = directory_temp();
		std::string tmp = dir;
		free(dir);

		return tmp;
	}
}
#endif
