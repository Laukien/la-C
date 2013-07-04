/*
 * =====================================================================================
 *
 *       Filename:  la_character.c
 *
 *    Description:  common character functions
 *
 *        Version:  1.0
 *        Created:  11/06/2012 12:35:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "la_character.h"

char character_fromHex(const char chr) {
	assert(chr >= '0' && chr <= 'f'); 
	assert(chr <= '9' || chr >= 'A'); 
	assert(chr <= 'F' || chr >= 'a'); 
	return isdigit(chr) ? (chr - '0') : (tolower(chr) - 'a' + 10);
}

char character_toHex(const char chr) {
	static char hex[] = "0123456789abcdef";
	return hex[chr & 15];
}

char *character_replace(const char *str, const char from, const char to) {
	assert(str != NULL);

	size_t len = strlen(str);
//	char *res = strdup(str);
	char *res = (char *) malloc(len + 1);
	if (res == NULL) return NULL;
	memset(res, '\0', len + 1);

	size_t i;
	for (i = 0; i < len; ++i) {
		if (str[i] == from)
			res[i] = to;
		else
			res[i] = str[i];
	}

	return res;
}

unsigned int character_count(const char *str, const char chr) {
	unsigned int i;
	unsigned int count = 0;

	for (i = 0; i < strlen(str); ++i) {
		if (str[i] == chr)
			++count;
	}

	return count;
}

#ifdef __cplusplus
namespace la {
	namespace character {
		char fromHex(const char chr) {
			return character_fromHex(chr);
		}

		char toHex(const char chr) {
			return character_toHex(chr);
		}

		std::string replace(const std::string &str, const char from, const char to) {
			char *tmp = character_replace(str.c_str(), from, to);
			std::string res = std::string(tmp);
			free(tmp);

			return res;
		}

		unsigned int count(const std::string &str, const char chr) {
			return character_count(str.c_str(), chr);
		}
	}
}
#endif
