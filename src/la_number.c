/*
 * =====================================================================================
 *
 *       Filename:  la_number.c
 *    Description:  number converting
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "la_message.h"
#include "la_number.h"

BOOL number_isNumber(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	unsigned int idx = 0;
	if (str[0] == '-')
		++idx;

	for (; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			return FALSE;
	}

	return TRUE;
}

BOOL number_isInteger(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	unsigned int idx = 0;
	int res = 0;
	int old = 0;
	if (str[0] == '-')
		++idx;

	for (; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			return FALSE;
		res *= 10;
        res += str[idx] - '0';                  /* ASCII */
		if (res < old)                          /* overflow */
			return FALSE;
		old = res;
	}

	return TRUE;
}

BOOL number_isUnsignedInteger(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	unsigned int idx;
	unsigned int res = 0;
	unsigned int old = 0;

	for (idx = 0; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			return FALSE;
		res *= 10;
        res += str[idx] - '0';                  /* ASCII */
		if (res < old)                          /* overflow */
			return FALSE;
		old = res;
	}

	return TRUE;
	
}

BOOL number_isSigned(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	return (str[0] == '-');
}

int number_toInteger(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	BOOL min = FALSE;
	unsigned int idx = 0;
	int res = 0;
	int old = 0;

	if (str[0] == '-') {
		min = TRUE;
		++idx;
	}

	for (; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			message_error("invalid format");
		res *= 10;
        res += str[idx] - '0';                  /* ASCII */
		if (res < old)                          /* overflow */
			message_error("integer overflow");
		old = res;
	}

	if (min) {
		old = res;
		res *= -1;                              /* add sign */
		if (res > old)
			message_error("integer overflow");
	}

	return res;
}

unsigned int number_toUnsignedInteger(const char *str) {
	assert(str != 0);
	assert(strlen(str) > 0);

	unsigned int idx = 0;
	unsigned int res = 0;
	unsigned int old = 0;

	for (idx = 0; idx < strlen(str); ++idx) {
        if (!isdigit(str[idx]))                 /* valid */
			message_error("invalid format");
		res *= 10;
        res += str[idx] - '0';                  /* ASCII */
		if (res < old)                          /* overflow */
			message_error("integer overflow");
		old = res;
	}

	return res;
}

size_t number_getIntegerLength(int num) {
	if (num == 0) return 1;

	/* get size (log10) */
	size_t size = 0;
	int i;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	if (num < 0) ++size;                        /* negative value */

	return size;
}

size_t number_getUnsignedIntegerLength(unsigned int num) {
	if (num == 0) return 1;

	/* get size (log10) */
	size_t size = 0;
	int i;
	for (i = num; i != 0; i /= 10) {
		++size;
	}

	return size;
}

char *number_integerToString(int num) {
	size_t size = number_getIntegerLength(num);

	char *str;
	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}

char *number_unsignedIntegerToString(unsigned int num) {
	size_t size = number_getUnsignedIntegerLength(num);

	char *str;
	str = (char *) malloc(size + 1);

	sprintf(str, "%d", num);

	return str;
}

#ifdef __cplusplus
namespace la {
	namespace number {
		bool isNumber(const std::string &str) {
			return number_isNumber(str.c_str());
		}

		bool isInteger(const std::string &str) {
			return number_isInteger(str.c_str());
		}

		bool isUnsignedInteger(const std::string &str) {
			return number_isUnsignedInteger(str.c_str());
		}

		bool isSigned(const std::string &str) {
			return number_isSigned(str.c_str());
		}

		int toInteger(const std::string &str) {
			return number_toInteger(str.c_str());
		}

		unsigned int toUnsignedInteger(const std::string &str) {
			return number_toUnsignedInteger(str.c_str());
		}

		size_t getLength(int num) {
			return number_getIntegerLength(num);
		}

		size_t getLength(unsigned int num) {
			return number_getUnsignedIntegerLength(num);
		}

		std::string toString(int num) {
			char *tmp = number_integerToString(num);
			std::string res = std::string(tmp);
			free(tmp);

			return res;
		}

		std::string toString(unsigned int num) {
			char *tmp = number_unsignedIntegerToString(num);
			std::string res = std::string(tmp);
			free(tmp);

			return res;
		}
	}
}
#endif
