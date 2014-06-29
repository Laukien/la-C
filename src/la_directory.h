/*
 * =====================================================================================
 *
 *       Filename:  la_directory.h
 *    Description:  I/O directory operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_DIRECTORY_H
#define LA_DIRECTORY_H

#include "la_boolean.h"
#include "la_list.h"
#include "la_system.h"

#ifdef __cplusplus
#include <string>

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
char *directory_name(const char *filename);
char *directory_temp();
LIST *directory_list(const char *directoryname, BOOL recursive);

#ifdef __cplusplus
}
namespace la {
	namespace directory {
		bool create(const std::string &name);
		bool exists(const std::string &name);
		std::string name(const std::string &filename);
		std::string temp();
		list::List list(const std::string &directoryname, bool recursive = true);
	}
}
#endif

#endif
