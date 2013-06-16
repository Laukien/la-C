#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif
#include "la_string.h"
#include "la_boolean.h"

BOOL boolean_isBoolean(const char *value) {
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

BOOL boolean_isTrue(const char *value) {
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
			tmp == 'y' ||
			tmp == '1';
	}

	/* one word */
	char *tmp = string_toLower(value);
	BOOL res =
		strcmp(tmp, "true") == 0 ||
		strcmp(tmp, "yes") == 0 ||
		strcmp(tmp, "on") == 0;
	free(tmp);

	return res;
}

BOOL boolean_isFalse(const char *value) {
	if (value == NULL)
		return FALSE;
	int len = strlen(value);

	if (len == 0)
		return FALSE;

	/* one character */
	if (len == 1) {
		char tmp = tolower(value[0]);
		return
			tmp == 'f' ||
			tmp == 'n' ||
			tmp == '0';
	}

	/* one word */
	char *tmp = string_toLower(value);
	BOOL res =
		strcmp(tmp, "false") == 0 ||
		strcmp(tmp, "no") == 0 ||
		strcmp(tmp, "off") == 0;
	free(tmp);

	return res;
}

BOOL boolean_toBoolean(const char *value) {
	if (value == NULL)
		return FALSE;

	/* one character */
	if (strlen(value) == 1) {
		return
			value[0] == 't' ||
			value[0] == 'y' ||
			value[0] == '1';
	}

	/* one word */
	char *tmp = string_toLower(value);
	BOOL res =
		strcmp(value, "true") == 0 ||
		strcmp(value, "yes") == 0 ||
		strcmp(value, "on") == 0;
	free (tmp);
	return res;
}

char *boolean_toString(BOOL value) {
	char *res;

	if (value)
		res = strdup("true");
	else
		res = strdup("false");
	
	return res;
}

#ifdef __cplusplus
	namespace la {
		namespace boolean {
			bool isBoolean(const std::string &value) {
				return boolean_isBoolean(value.c_str());
			}

			bool isTrue(const std::string &value) {
				return boolean_isTrue(value.c_str());
			}

			bool isFalse(const std::string &value) {
				return boolean_isFalse(value.c_str());
			}

			bool toBoolean(const std::string &value) {
				return boolean_toBoolean(value.c_str());
			}

			std::string toString(bool value) {
				char *tmp = boolean_toString(value);
				std::string res = tmp;
				free(tmp);

				return res;
			}
		}
	}
#endif
