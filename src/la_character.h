/*
 * =====================================================================================
 *
 *       Filename:  la_character.h
 *    Description:  character functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_CHARACTER
#define LA_CHARACTER

#include <la_boolean.h>

#ifdef __cplusplus
extern "C" {
#endif

char character_fromHex(const char chr);
char character_toHex(const char chr);
char character_fromBase64(const char chr);
char character_toBase64(const char chr);
BOOL character_isBase64(const char chr);
char *character_replace(const char *str, const char from, const char to);
unsigned int character_count(const char *str, const char chr);

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace character {
		char fromHex(const char chr);
		char toHex(const char chr);
		char fromBase64(const char chr);
		char toBase64(const char chr);
		bool isBase64(const char chr);
		std::string replace(const std::string &str, const char from, const char to);
		unsigned int count(const std::string &str, const char chr);
	}
}
#endif

#endif
