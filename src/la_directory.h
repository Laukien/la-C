/*
 * =====================================================================================
 *
 *       Filename:  la_directory.h
 *
 *    Description:  header for la_directory.c
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

#ifndef LA_DIRECTORY_H
#define LA_DIRECTORY_H

#ifdef __cplusplus
#include <string>
namespace directory {
	bool create(const std::string name);
	bool exists(const std::string name);
	std::string temp();
}
extern "C" {
#endif

#include "la_common.h"
#include "la_list.h"
#include "la_system.h"

#ifdef SYSTEM_OS_VENDOR_MICROSOFT
	#define DIRECTORY_SEPARATOR_CHAR '\\'
	#define DIRECTORY_SEPARATOR_STRING "\\"
#else	
	#define DIRECTORY_SEPARATOR_CHAR '/'
	#define DIRECTORY_SEPARATOR_STRING "/"
#endif

BOOL directory_create(const char *directoryname);
BOOL directory_exists(const char *directoryname);
char *directory_temp();
LIST *directory_list(const char *directoryname);

#ifdef __cplusplus
}
#endif

#endif
