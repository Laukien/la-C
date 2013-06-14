#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif
#include "la_string.h"
#include "la_boolean.h"

BOOL boolean_isValid(const char *value) {
	if (value == NULL)
		return FALSE;
	int len = strlen(value);

	if (len == 0)
		return FALSE;

	/* one character */
	if (len == 1) {
		char tmp = tolower(value[0]);
		return
			tmp == 't' ||
			tmp == 'f' ||
			tmp == 'y' ||
			tmp == 'n' ||
			tmp == '1' ||
			tmp == '0';
	}

	/* one word */
	char *tmp = string_toLower(value);
	BOOL res =
		strcmp(tmp, "true") == 0 ||
		strcmp(tmp, "false") == 0||
		strcmp(tmp, "yes") == 0 ||
		strcmp(tmp, "no") == 0 ||
		strcmp(tmp, "on") == 0 ||
		strcmp(tmp, "off") == 0;
	free(tmp);

	return res;
}

#ifdef __cplusplus
	namespace la {
		namespace boolean {
//			bool isValid(const std::string &value) {
//				return boolean_isValid(value.c_str());
//			}
		}
	}
#endif
