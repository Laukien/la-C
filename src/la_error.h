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

#include "la_boolean.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_NONE 0
#define ERROR_UNKNOWN -1

#define ERROR_MESSAGE_SIZE 1024

typedef struct la_error ERROR;

ERROR *error_new();
void error_free(ERROR *self);
void error_set(ERROR *self, int id, const char *txt, ...);
void error_setId(ERROR*self, int id);
int error_getId(ERROR *self);
void error_setMessage(ERROR*self, const char *txt, ...);
char *error_getMessage(ERROR *self);
void error_setComment(ERROR*self, const char *txt);
char *error_getComment(ERROR *self);
void error_setCause(ERROR*self, const char *txt);
char *error_getCause(ERROR *self);
void error_setAction(ERROR*self, const char *txt);
char *error_getAction(ERROR *self);
void error_show(ERROR *self);
BOOL error_exists(ERROR *self);
void error_reset(ERROR *self);

#ifdef __cplusplus
}

#include <string>

namespace la {
	class error {
		private:
			ERROR *obj;
		public:
			error();
			~error();
			void set(int id, const std::string &txt);
			void setId(int id);
			int getId();
			void setMessage(std::string &txt);
			std::string getMessage();
			void setComment(std::string &txt);
			std::string getComment();
			void setCause(std::string &txt);
			std::string getCause();
			void setAction(std::string &txt);
			std::string getAction();
			void show();
			bool exists();
			void reset();
	};
}
#endif

#endif
