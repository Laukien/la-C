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
#include <dirent.h>

BOOL directory_create(const char *directoryname) {
	if (directory_exists(directoryname)) return TRUE;

	char *dir = strdup(directoryname);
	size_t i;

	/* make directory tokens */
	for (i = 1; i < strlen(directoryname); ++i) {
		if (dir[i] == DIRECTORY_SEPARATOR_CHAR) dir[i] = '\0';
	}

	/* rebuild directory from the root */
	for (i = 1; i < strlen(directoryname); ++i) {
		if (dir[i] != '\0') continue;
		if (!directory_exists(directoryname)) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
			mkdir(dir);
#else
			mkdir(dir, 0755);
#endif

		}
		dir[i] = DIRECTORY_SEPARATOR_CHAR;
	}

#ifdef SYSTEM_OS_TYPE_WINDOWS
			mkdir(directoryname);
#else
			mkdir(directoryname, 0755);
#endif

	free(dir);

	return (directory_exists(directoryname));
}

BOOL directory_exists(const char *name) {
	struct stat st;
	if (!stat(name, &st) == 0) {
		return FALSE;
	}
	return (st.st_mode & S_IFDIR);
}

char *directory_name(const char *filename) {
	int i;
	for (i = strlen(filename)-1; i >= -1; i--) {
		if (i == -1) break;                     /* not found */
		if (filename[i] == DIRECTORY_SEPARATOR_CHAR) break;
	}

    int l = i;                                  /* len */
	char *name = (char *) malloc(l + 1);
	if(name == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(name, filename, l);
	name[l] = '\0';

	return name;
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

LIST *directory_list(const char *directoryname) {
	DIR *dp;	
	struct dirent *ep;
	struct stat st;
	char *filename;
	LIST *names = list_new();

	dp = opendir(directoryname);
	if (dp != NULL) {
		while ((ep = readdir(dp)) != NULL) {
			/* check DOT-path */
			if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

			/* build filename */
			filename = (char *) malloc(strlen(directoryname) + 1 + strlen(ep->d_name) + 1);
			strcpy(filename, directoryname);
			strcat(filename, DIRECTORY_SEPARATOR_STRING);
			strcat(filename, ep->d_name);
			stat(filename, &st);
			free(filename);

			if (st.st_mode & S_IFDIR)
				list_add(names, ep->d_name);
		}
		closedir(dp);
	}

	return names;
}

#ifdef __cplusplus
namespace la {
	namespace directory {
		bool create(const std::string &name) {
			return directory_create(name.c_str());
		}

		bool exists(const std::string &name) {
			return directory_exists(name.c_str());
		}

		std::string name(const std::string &filename) {
			std::string res;
			char *name = directory_name(filename.c_str());
			res = name;
			free(name);

			return res;
		}

		std::string temp() {
			char *dir = directory_temp();
			std::string tmp = dir;
			free(dir);

			return tmp;
		}

		la::list list(const std::string &directoryname) {
			LIST *l = directory_list(directoryname.c_str());

			return la::list(l);
		}
	}
}
#endif
