#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "la_message.h"

void message_error(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;
	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	fprintf(stderr, "ERROR:\t\t%s\n", buf);

	va_end(args);

	exit(EXIT_FAILURE);
}

void message_debug(const char *text, ...) {
	char *env = getenv("DEBUG");
	if (env != NULL && strlen(env) > 0) {
		char c = tolower(env[0]);
		if (c == 'y' || c == 't' || c == '1') {
			char buf[MESSAGE_SIZE];
			va_list args;
			va_start(args, text);
			vsnprintf(buf, MESSAGE_SIZE, text, args);

			printf("DEBUG:\t\t%s\n", text);

			va_end(args);
		}
	}
}

void message_info(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;
	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	printf("INFO:\t\t%s\n", text);

	va_end(args);
}

void message_warn(const char *text, ...) {
	char buf[MESSAGE_SIZE];
	va_list args;
	va_start(args, text);
	vsnprintf(buf, MESSAGE_SIZE, text, args);

	printf("WARNING:\t%s\n", text);

	va_end(args);
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
