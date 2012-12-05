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
#ifdef SYSTEM_ARCH_64
	#ifndef __USE_LARGEFILE64
		#define __USE_LARGEFILE64
	#endif
	#ifndef _LARGEFILE_SOURCE
		#define _LARGEFILE_SOURCE
	#endif
	#ifndef _LARGEFILE64_SOURCE
		#define _LARGEFILE64_SOURCE
	#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

BOOL file_exists(const char *filename) {
	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return FALSE;

	fclose(pFile);
	return TRUE;
}

BOOL file_remove(const char *filename) {
	int ret = unlink(filename);
	return ret ? FALSE : TRUE;
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

size_t file_size(const char *filename) {
#ifdef SYSTEM_ARCH_64
	struct stat64 st;
	if (stat64(filename, &st) != 0) return -1;
#else
	struct stat st;
	if (stat(filename, &st) != 0) return -1;
#endif
	size_t size = st.st_size;

	return size;
}

void _file_list(const char *directoryname, LIST *list) {
	DIR *dp;	
	struct dirent *ep;
	struct stat st;
	char *filename;
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

			if (st.st_mode & S_IFDIR) _file_list(filename, list);
			list_add(list, filename);
		}
		closedir(dp);
	}
}

LIST *file_list(const char *filename) {
	LIST *list = list_new();

	_file_list(filename, list);

	return list;
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
