/*
 * =====================================================================================
 *
 *       Filename:  la_file.h
 *
 *    Description:  header for la_file.c
 *
 *        Version:  1.0
 *        Created:  11/11/2011 02:26:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_FILE_H
#define LA_FILE_H

#include "la_boolean.h"
#include "la_exception.h"
#include "la_list.h"
#include <string.h>

#ifdef __cplusplus
#include <string>

extern "C" {
#endif

typedef void (*FILE_PROCESS)(const char *from, const char *to, size_t size, size_t done);

BOOL file_exists(const char *filename);
BOOL file_remove(const char *filename);
char *file_name(const char *file);
char *file_temp();
size_t file_size(const char *filename);
LIST *file_list(const char *directoryname, BOOL recursive);
char *file_extension(const char *filename);
void FILE_PROCESS_SIMPLE(const char *from, const char *to, size_t size, size_t done);
void FILE_PROCESS_DOT(const char *from, const char *to, size_t size, size_t done);
void FILE_PROCESS_PERCENT(const char *from, const char *to, size_t size, size_t done);
BOOL file_copy(const char *from, const char *to, FILE_PROCESS proc, EXCEPTION *e);

#ifdef __cplusplus
}
namespace la {
	namespace file {
		bool exists(const std::string &filename);
		bool remove(const std::string &filename);
		std::string name(const std::string &filename);
		std::string temp();
		size_t size(const std::string &filename);
		list::List list(const std::string &directoryname, bool recursive = true);
		std::string extension(const std::string &filename);
		bool copy(const std::string &from, const std::string &to, FILE_PROCESS proc = NULL);
	}
}
#endif

#endif
