/*
 * =====================================================================================
 *
 *       Filename:  la_error.h
 *
 *    Description:  definition to handle errors
 *
 *        Version:  1.0
 *        Created:  04/05/2012 17:29:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_ERROR_H
#define LA_ERROR_H

#include "la_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_MESSAGE_SIZE 1024

#define ERROR_NONE 0
#define ERROR_UNKNOWN -1

void error_init();
void error_set(int id, const char *msg);
int error_getId();
char *error_getMessage();
void error_show();
BOOL error_exists();
void error_reset();

#ifdef __cplusplus
}

#include <string>

namespace la {
	namespace error {
		void init();
		void set(int id, const std::string &msg);
		int getId();
		std::string getMessage();
		void show();
		bool exists();
		void reset();
	}
}
#endif

#endif
