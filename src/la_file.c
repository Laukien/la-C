/*
 * =====================================================================================
 *
 *       Filename:  la_file.c
 *    Description:  I/O file operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include "la_file.h"
#include "la_directory.h"
#include "la_number.h"
#include "la_string.h"
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
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define FILE_BUFFER_SIZE 1024

BOOL file_exists(const char *filename) {
/* 
	FILE *pFile = fopen(filename, "r");
	if ( pFile == NULL) return FALSE;

	fclose(pFile);
	return TRUE;
*/

	struct stat st;
	if (!stat(filename, &st) == 0) {
		return FALSE;
	}
	return (st.st_mode & S_IFREG);
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
	char *pid_s = number_integerToString(pid);
	int rnd = rand();
	char *rnd_s = number_integerToString(rnd);

	char *dir = directory_temp();	
	size_t size = strlen(dir) + 1 + strlen(pid_s) + 1 + strlen(rnd_s);
	dir = (char *) realloc(dir, size + 1);
	strcat(dir, DIRECTORY_SEPARATOR_STRING);
	strcat(dir, pid_s);
	strcat(dir, "_");
	strcat(dir, rnd_s);

	free(rnd_s);
	free(pid_s);

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

void _file_list(const char *directoryname, BOOL recursive, LIST *list) {
	DIR *dp;	
	struct dirent *ep;
	struct stat st;
	char *name;
	size_t len = strlen(directoryname);
	dp = opendir(directoryname);
	if (dp != NULL) {
		while ((ep = readdir(dp)) != NULL) {
			/* check DOT-path */
			if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;

			/* build filename */
			name = (char *) malloc(len + 1 + strlen(ep->d_name) + 1);
			strcpy(name, directoryname);
			strcat(name, DIRECTORY_SEPARATOR_STRING);
			strcat(name, ep->d_name);
			stat(name, &st);

			/* call its self */
			if (recursive && (st.st_mode & S_IFDIR)) {
				_file_list(name, TRUE, list);
				free(name);
				continue;
			}

			if (st.st_mode & S_IFREG) {
				list_add(list, name);
			}

			free(name);
		}
		closedir(dp);
	}
}

LIST *file_list(const char *directoryname, BOOL recursive) {
	LIST *list = list_new();

	_file_list(directoryname, recursive, list);

	return list;
}

char *file_extension(const char *filename) {
	assert(filename);

	char *sep = strrchr((char *)filename, '.');
	if (!sep || sep == filename + strlen(filename) - 1) {
		return NULL;
	}

	char *ext = string_trim(sep + 1);
	
	BOOL ok = string_isAlnum(ext);
	if (!ok) {
		free(ext);
		return NULL;
	}

	return ext;
}

BOOL file_copy(const char *from, const char *to, FILE_PROCESS proc, EXCEPTION *e) {
	assert(from);
	assert(to);

	/* open file to read */
	int fd_r;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	fd_r = open(from, O_RDONLY|O_BINARY);
#else
	fd_r = open(from, O_RDONLY);
#endif
	if (fd_r == -1) {
		return FALSE;
	}

	/* open file to write */
	int fd_w;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	fd_w = open(to, O_WRONLY|O_CREAT|O_BINARY, 0644);
#else
	fd_w = open(to, O_WRONLY|O_CREAT, 0644);
#endif
	if (fd_w == -1) {
		close(fd_r);
		return FALSE;
	}

	/* get size */
	size_t size = lseek(fd_r, 0L, SEEK_END);
	lseek(fd_r, 0L, SEEK_SET);
	size_t done = 0;

	/* call process */
	if (proc) {
		proc(from, to, size, done);
	}

	/* copy */
	char *buf[FILE_BUFFER_SIZE];
	int rc;
	while ((rc = read(fd_r, buf, FILE_BUFFER_SIZE))) {
		if (rc < 0) {
			close(fd_w);
			close(fd_r);
			return FALSE;
		}
		write(fd_w, buf, rc);

		done += rc;

		/* call process */
		if (proc) {
			proc(from, to, size, size);
		}

	}

	/* close file descriptors */
	close(fd_w);
	close(fd_r);

	return TRUE;
}

void FILE_PROCESS_SIMPLE(const char *from, const char *to, size_t size, size_t done) {
	if (done == 0) {
		printf ( "%s -> %s\n", from, to );
	}
}

void FILE_PROCESS_DOT(const char *from, const char *to, size_t size, size_t done) {
	if (done == 0) {
		printf ( "%s -> %s ", from, to );
	}

	printf ( "." );
	fflush(stdout);

	if (done == size) {
		printf ( "\n" );
	}
}

void FILE_PROCESS_PERCENT(const char *from, const char *to, size_t size, size_t done) {
	printf ( "\r%s -> %s: %3d%%", from, to, (int)(done * 100 / size) );
	fflush(stdout);
	if (done == size) {
		printf ( "\n" );
	}
}

#ifdef __cplusplus
namespace la {
	namespace file {
		bool exists(const std::string &filename) {
			return file_exists(filename.c_str());
		}

		bool remove(const std::string &filename) {
			return file_remove(filename.c_str());
		}

		std::string name(const std::string &filename) {
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

		size_t size(const std::string &filename) {
			return file_size(filename.c_str());
		}

		la::list list(const std::string &directoryname, bool recursive) {
			LIST *l = file_list(directoryname.c_str(), recursive);

			return la::list(l);
		}

		std::string extension(const std::string &filename) {
			char *tmp = file_extension(filename.c_str());
			if (!tmp) {
				return std::string();
			}

			std::string res = tmp;
			free(tmp);

			return res;
		}

		bool copy(const std::string &from, const std::string &to, FILE_PROCESS proc) {
			return file_copy(from.c_str(), to.c_str(), proc, NULL);
		}
	}
}
#endif
