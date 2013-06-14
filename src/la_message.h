#ifndef LA_MESSAGE_H
#define LA_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

void message_error(const char *text);
void message_debug(const char *text);
void message_info(const char *text);
void message_warn(const char *text);

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace message {
		void error(const std::string &text);
		void debug(const std::string &text);
		void info(const std::string &text);
		void warn(const std::string &text);
	}
}
#endif

#endif
