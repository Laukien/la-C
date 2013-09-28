/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *
 *    Description:  Header for la_string.c
 *
 *        Version:  1.0
 *        Created:  11/07/2011 01:20:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef LA_STRING_H
#define LA_STRING_H

#include "la_boolean.h"
#include "la_list.h"
#include "la_system.h"
#ifdef SYSTEM_OS_TYPE_LINUX
#include <regex.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

char *string_toLower(const char *str);
char *string_toUpper(const char *str);
char *string_trim(const char *str);
char *string_replaceFirst(const char *string, const char *from, const char *to);
char *string_replace(const char *string, const char *from, const char *to);
LIST *string_split(const char *string, const char *delimiters);
BOOL string_isEmpty(const char *string);
char *string_loadFromFile(const char *filename);
BOOL string_saveToFile(const char *filename, const char *str);
#ifdef SYSTEM_OS_TYPE_LINUX
char *string_regexp (const char *string, const char *pattern, int *begin, int *end);
#endif
char *string_fromHex(const char *string);
char *string_toHex(const char *string);

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace string {
		std::string toLower(const std::string &string);
		std::string toUpper(const std::string &string);
		std::string trim(const std::string &string);
		std::string replaceFirst(const std::string &string, const std::string &from, const std::string &to);
		std::string replace(const std::string &string, const std::string &from, const std::string &to);
		list split(const std::string &string, const std::string &delimiters);
		bool isEmpty(const std::string &string);
		std::string loadFromFile(const std::string &filename);
		bool saveToFile(const std::string &filename, const std::string &string);
	#ifdef SYSTEM_OS_TYPE_LINUX
		std::string regexp(const std::string &string, const std::string &pattern, int &begin, int &end);
	#endif
		std::string fromHex(const std::string &string);
		std::string toHex(const std::string &string);

	}
}
#endif

#endif
