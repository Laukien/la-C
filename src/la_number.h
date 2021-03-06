/*
 * =====================================================================================
 *
 *       Filename:  la_number.h
 *    Description:  number converting
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_NUMBER_H
#define LA_NUMBER_H

#include <string.h>
#include "la_boolean.h"

#ifdef __cplusplus
#include <string>

extern "C" {
#endif

BOOL number_isNumber(const char *str);
BOOL number_isInteger(const char *str);
BOOL number_isUnsignedInteger(const char *str);
BOOL number_isSigned(const char *str);
int number_toInteger(const char *str);
unsigned int number_toUnsignedInteger(const char *str);
size_t number_getIntegerLength(int num);
size_t number_getUnsignedIntegerLength(unsigned int num);
char *number_integerToString(int num);
char *number_unsignedIntegerToString(unsigned int num);

#ifdef __cplusplus
}

namespace la {
	namespace number {
		bool isNumber(const std::string &str);
		bool isInteger(const std::string &str);
		bool isUnsignedInteger(const std::string &str);
		bool isSigned(const std::string &str);
		int toInteger(const std::string &str);
		unsigned int toUnsignedInteger(const std::string &str);
		size_t getLength(int num);
		size_t getLength(unsigned int num);
		std::string toString(int num);
		std::string toString(unsigned int num);
	}
}
#endif

#endif
