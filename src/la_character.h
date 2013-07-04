/*
 * =====================================================================================
 *
 *       Filename:  la_character.h
 *
 *    Description:  Header for la_character.c
 *
 *        Version:  1.0
 *        Created:  11/06/2012 12:35:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_CHARACTER
#define LA_CHARACTER

#ifdef __cplusplus
extern "C" {
#endif

char character_fromHex(const char chr);
char character_toHex(const char chr);
char *character_replace(const char *str, const char from, const char to);
unsigned int character_count(const char *str, const char chr);

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace character {
		char fromHex(const char chr);
		char toHex(const char chr);
		std::string replace(const std::string &str, const char from, const char to);
		unsigned int count(const std::string &str, const char chr);
	}
}
#endif

#endif
