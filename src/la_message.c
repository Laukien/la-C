#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "la_message.h"

void message_error(const char *text) {
	fprintf(stderr, "%16s%s\n", "ERROR:", text);
	exit(EXIT_FAILURE);
}

void message_debug(const char *text) {
	char *env = getenv("DEBUG");
	if (env != NULL && strlen(env) > 0) {
		char c = tolower(env[0]);
		if (c == 'y' || c == 't' || c == '1')
			printf("%16s%s\n", "DEBUG:", text);
	}
}
void message_info(const char *text) {
	printf("%16s%s\n", "INFO:", text);
}

void message_warn(const char *text) {
	printf("%16s%s\n", "WARNING:", text);
}

#ifdef __cplusplus
namespace la {
	namespace message {
		void error(const std::string &text) {
			message_error(text.c_str());
		}

		void debug(const std::string &text) {
			message_debug(text.c_str());
		}

		void info(const std::string &text) {
			message_info(text.c_str());
		}

		void warn(const std::string &text) {
			message_warn(text.c_str());
		}
	}
}
#endif
