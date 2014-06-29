/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *    Description:  common string operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
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
#include <string>

extern "C" {
#endif

char *string_toLower(const char *str);
char *string_toUpper(const char *str);
char *string_toCamel(const char *str);
char *string_trim(const char *str);
char *string_replaceFirst(const char *string, const char *from, const char *to);
char *string_replace(const char *string, const char *from, const char *to);
LIST *string_split(const char *string, const char *delimiters);
BOOL string_isEmpty(const char *string);
BOOL string_isAlnum(const char *str);
BOOL string_isAlpha(const char *str);
BOOL string_isCntrl(const char *str);
BOOL string_isDigit(const char *str);
BOOL string_isGraph(const char *str);
BOOL string_isLower(const char *str);
BOOL string_isPrint(const char *str);
BOOL string_isPunct(const char *str);
BOOL string_isSpace(const char *str);
BOOL string_isUpper(const char *str);
BOOL string_isXdigit(const char *str);
char *string_loadFromFile(const char *filename);
BOOL string_saveToFile(const char *filename, const char *str);
#ifdef SYSTEM_OS_TYPE_LINUX
char *string_regexp (const char *string, const char *pattern, int *begin, int *end);
#endif
char *string_fromHex(const char *string);
char *string_toHex(const char *string);

#ifdef __cplusplus
}

namespace la {
	namespace string {
		std::string toLower(const std::string &string);
		std::string toUpper(const std::string &string);
		std::string toCamel(const std::string &string);
		std::string trim(const std::string &string);
		std::string replaceFirst(const std::string &string, const std::string &from, const std::string &to);
		std::string replace(const std::string &string, const std::string &from, const std::string &to);
		list::List split(const std::string &string, const std::string &delimiters);
		bool isEmpty(const std::string &string);
		bool isAlnum(const std::string &str);
		bool isAlpha(const std::string &str);
		bool isCntrl(const std::string &str);
		bool isDigit(const std::string &str);
		bool isGraph(const std::string &str);
		bool isLower(const std::string &str);
		bool isPrint(const std::string &str);
		bool isPunct(const std::string &str);
		bool isSpace(const std::string &str);
		bool isUpper(const std::string &str);
		bool isXDigit(const std::string &str);
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
