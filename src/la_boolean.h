#ifndef LA_BOOLEAN_H
#define LA_BOOLEAN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL
	#define BOOL int
	#define TRUE 1
	#define FALSE 0 
#endif

BOOL boolean_isBoolean(const char *value);
BOOL boolean_toBoolean(const char *value);
BOOL boolean_isTrue(const char *value);
BOOL boolean_isFalse(const char *value);
char *boolean_toString(BOOL value);

#ifdef __cplusplus
}

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>

namespace la {
	namespace boolean {
		bool isBoolean(std::string &value);
		bool isTrue(const std::string &value);
		bool isFalse(const std::string &value);
		bool toBoolean(std::string &value);
		std::string toString(bool value);
	}
}
#endif

#endif
