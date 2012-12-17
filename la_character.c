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
	char *res = (char *) malloc(len + 1);

	size_t i;
	for (i = 0; i < len; ++i) {
		if (str[i] == from)
			res[i] = to;
		else
			res[i] = str[i];
	}

	return res;
}
