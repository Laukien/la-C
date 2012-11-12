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
#include <string.h>

#ifdef __cplusplus
#include <string>
namespace file {
	bool exists(const std::string filename);
	std::string name(const std::string filename);
	std::string temp();
}
extern "C" {
#endif

#include "la_common.h"
#include "la_list.h"

BOOL file_exists(const char *filename);
char *file_name(const char *file);
char *file_temp();
size_t file_size(const char *filename);
LIST *file_list(const char *directoryname);

#ifdef __cplusplus
}
#endif

#endif
