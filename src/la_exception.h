/*
 * =====================================================================================
 *
 *       Filename:  la_boolean.h
 *    Description:  exception handling
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_EXCEPTION_H
#define LA_EXCEPTION_H

#include "la_boolean.h"

#ifdef __cplusplus
#include <string>

extern "C" {
#endif

#define EXCEPTION_CALLBACK_SIZE 16
#define EXCEPTION_NONE 0
#define EXCEPTION_UNKNOWN -1

#define EXCEPTION_MESSAGE_SIZE 1024

typedef struct la_exception EXCEPTION;
typedef void (*EXCEPTION_CALLBACK)(EXCEPTION * self, void *pointer);

EXCEPTION *exception_new();
void exception_free(EXCEPTION *e);
void exception_addCallback(EXCEPTION *e, EXCEPTION_CALLBACK callback, void *pointer);
void exception_delCallback(EXCEPTION *e);
BOOL exception_isCallback(EXCEPTION *e);
void exception_throwCallback(EXCEPTION *e);
void exception_setShort(EXCEPTION *self, int id, const char *message, ...);
void exception_setLong(EXCEPTION *self, int id, const char *message, const char *cause, const char *action);
void exception_setId(EXCEPTION *self, int id);
int exception_getId(EXCEPTION *self);
void exception_setMessage(EXCEPTION *self, const char *txt, ...);
char *exception_getMessage(EXCEPTION *self);
void exception_setCause(EXCEPTION *self, const char *txt);
char *exception_getCause(EXCEPTION *self);
void exception_setAction(EXCEPTION *self, const char *txt);
char *exception_getAction(EXCEPTION *self);
void exception_setComment(EXCEPTION *self, const char *txt);
char *exception_getComment(EXCEPTION *self);
void exception_show(EXCEPTION *self);
BOOL exception_exists(EXCEPTION *e);
void exception_reset(EXCEPTION *self);
void exception_throw(EXCEPTION *e);

#ifdef __cplusplus
}

namespace la {
	namespace exception {
		class Exception {
			private:
				EXCEPTION *obj;
			public:
				Exception();
				Exception(EXCEPTION *e);
				Exception(int id, const std::string &message);
				Exception(int id, const std::string &message, const std::string &cause, const std::string &action);
				~Exception();
				void setShort(int id, const std::string &message);
				void setLong(int id, const std::string &message, const std::string &cause = "", const std::string &action = "");
				void setId(int id);
				int getId();
				void setMessage(std::string &txt);
				std::string getMessage();
				void setCause(std::string &txt);
				std::string getCause();
				void setAction(std::string &txt);
				std::string getAction();
				void setComment(std::string &txt);
				std::string getComment();
				void show();
				bool exists();
				void reset();
		};
	}
}
#endif

#endif
