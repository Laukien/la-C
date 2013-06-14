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

BOOL boolean_isValid(const char *value);
//BOOL boolean_isTrue(const char *value);
//BOOL boolean_isFalse(const char *value);

#ifdef __cplusplus
}

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>

namespace la {
	namespace boolean {
		bool isValid(std::string value);
//		isTrue(const string &value);
//		isFalse(const string &value);
	}
}
#endif

#endif
