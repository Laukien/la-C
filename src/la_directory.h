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

#include "la_common.h"
#include "la_list.h"
#include "la_system.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#include <string>
namespace la {
	namespace directory {
		bool create(const std::string &name);
		bool exists(const std::string &name);
		std::string temp();
		la::list list(const std::string &directoryname);
	}
}
#endif

#endif
