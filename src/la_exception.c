/*
 * =====================================================================================
 *
 *       Filename:  exception.c
 *
 *    Description:  Exception handeling
 *
 *        Version:  1.0
 *        Created:  07/01/2012 06:39:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "la_message.h"
#include "la_exception.h"

struct la_exception {
	int id;
	char *message;
	char *cause;
	char *action;
	char *comment;
	int level;
	EXCEPTION_CALLBACK callback[EXCEPTION_CALLBACK_SIZE];
	void *pointer[EXCEPTION_CALLBACK_SIZE];
};

EXCEPTION *exception_new() {
	EXCEPTION *self = (EXCEPTION *)malloc(sizeof(EXCEPTION));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, '\0', sizeof(EXCEPTION));
	self->id = EXCEPTION_NONE;
	self->level = 0;

	return self;
}

void exception_free(EXCEPTION *self) {
	assert(self);

	exception_reset(self);
	free(self);
	self = NULL;
}

void exception_addCallback(EXCEPTION *self, EXCEPTION_CALLBACK callback, void *pointer) {
	assert(self);

	if (self->level >= EXCEPTION_CALLBACK_SIZE) {
		fprintf(stderr, "ERROR - Exception: callback-level to deep\n");
		exit(EXIT_FAILURE);
	}

	self->callback[self->level] = callback;
	self->pointer[self->level] = pointer;
	++(self->level);
}

void exception_delCallback(EXCEPTION *self) {
	assert(self);

	if (self->level <= 0) {
		fprintf(stderr, "ERROR - Exception: no callback to delete\n");
		exit(EXIT_FAILURE);
	}

	--(self->level);
}

BOOL exception_isCallback(EXCEPTION *self) {
	assert(self);

	return self->level ? TRUE : FALSE;
}

void exception_throwCallback(EXCEPTION *self) {
	assert(self);

	int i;
	for (i = self->level - 1; i >= 0; --i) {
		self->callback[i](self, self->pointer[i]);
	}
}

int exception_level(EXCEPTION *self) {
	assert(self);

	return self->level;
}

void exception_setShort(EXCEPTION *self, int id, const char *message, ...) {
	assert(self);

	/* set id */
	self->id = id;

	/* set message */
	if (self->message) {
		free(self->message);
		self->message = NULL;
	}
	if (message) {
		/* if no 'id' has been set, set an unknown error */
		if (self->id == EXCEPTION_NONE) {
			self->id = EXCEPTION_UNKNOWN;
		}

		va_list args;
		char buf[EXCEPTION_MESSAGE_SIZE];
		va_start(args, message);
		vsnprintf(buf, EXCEPTION_MESSAGE_SIZE, message, args);
		va_end(args);
		self->message = strdup(buf);
	}
}

void exception_setLong(EXCEPTION *self, int id, const char *message, const char *cause, const char *action) {
	assert(self);

	exception_reset(self);

	self->id = (id != EXCEPTION_NONE ? id : EXCEPTION_UNKNOWN);
	if (message) {
		self->message = strdup(message);
	}
	if (cause) {
		self->cause = strdup(cause);
	}
	if (action) {
		self->action = strdup(action);
	}
}

void exception_setId(EXCEPTION *self, int id) {
	assert(self);

	self->id = id;
}

int exception_getId(EXCEPTION *self) {
	assert(self);

	return self->id;
}

void exception_setMessage(EXCEPTION *self, const char *txt, ...) {
	assert(self);
	assert(txt);

	/* set message */
	if (self->message) {
		free(self->message);
		self->message = NULL;
	}
	if (txt) {
		/* if no 'id' has been set, set an unknown error */
		if (self->id == EXCEPTION_NONE) {
			self->id = EXCEPTION_UNKNOWN;
		}

		va_list args;
		char buf[EXCEPTION_MESSAGE_SIZE];
		va_start(args, txt);
		vsnprintf(buf, EXCEPTION_MESSAGE_SIZE, txt, args);
		va_end(args);
		self->message = strdup(buf);
	}
}

char *exception_getMessage(EXCEPTION *self) {
	assert(self);

	return self->message;
}

void exception_setCause(EXCEPTION *self, const char *txt) {
	assert(self);

	/* reset */
	if (self->cause) {
		free(self->cause);
		self->cause = NULL;
	}

	if (txt) {
		self->cause = strdup(txt);
	}
}

char *exception_getCause(EXCEPTION *self) {
	assert(self);

	return self->cause;
}

void exception_setAction(EXCEPTION *self, const char *txt) {
	assert(self);

	/* reset */
	if (self->action) {
		free(self->action);
		self->action = NULL;
	}

	if (txt) {
		self->action = strdup(txt);
	}
}

char *exception_getAction(EXCEPTION *self) {
	assert(self);

	return self->action;
}

void exception_setComment(EXCEPTION *self, const char *txt) {
	assert(self);

	/* reset */
	if (self->comment) {
		free(self->comment);
		self->comment = NULL;
	}

	if (txt) {
		self->comment = strdup(txt);
	}
}

char *exception_getComment(EXCEPTION *self) {
	assert(self);

	return self->comment;
}

BOOL exception_exists(EXCEPTION *self) {
	assert(self);

	return self->id != EXCEPTION_NONE;
}

void exception_reset(EXCEPTION *self) {
	assert(self);

	self->id = EXCEPTION_NONE;
	if (self->message) {
		free(self->message);
		self->message = NULL;
	}
	if (self->comment) {
		free(self->comment);
		self->comment = NULL;
	}
	if (self->cause) {
		free(self->cause);
		self->cause = NULL;
	}
	if (self->action) {
		free(self->action);
		self->action = NULL;
	}
}

void exception_show(EXCEPTION *self) {
	assert(self);

	fprintf (stderr, "EXCEPTION:\t%s (%d)\n", self->message, self->id);
	if (self->cause) {
		fprintf (stderr, "    CAUSE:\t%s\n", self->cause);
	}
	if (self->action) {
		fprintf (stderr, "   ACTION:\t%s\n", self->action);
	}
	if (self->comment) {
		fprintf (stderr, "  COMMENT:\t%s\n", self->comment);
	}
}

void exception_throw(EXCEPTION *self) {
	assert(self);

	if (exception_exists(self)) {
		if (exception_isCallback(self)) {
			exception_throwCallback(self);
		} else {
			exception_show(self);
			exception_free(self);
			exit(EXIT_FAILURE);
		}
	}
}

#ifdef __cplusplus
namespace la {
	exception::exception() {
		this->obj = exception_new();
	}

	exception::~exception() {
		exception_free(this->obj);
	}

	void exception::setAll(int id, const std::string &message, const std::string &cause, const std::string &action) {
		exception_setAll(this->obj, id, message.c_str(), (cause.empty() ? NULL : cause.c_str()), (action.empty() ? NULL : action.c_stR()));
	}

	void exception::setId(int id) {
		exception_setId(this->obj, id);
	}

	int exception::getId() {
		return exception_getId(this->obj);
	}

	void exception::setMessage(std::string &txt) {
		exception_setMessage(this->obj, txt.c_str());
	}

	std::string exception::getMessage() {
		return std::string(exception_getMessage(this->obj));
	}

	void exception::setCause(std::string &txt) {
		exception_setCause(this->obj, txt.c_str());
	}

	std::string exception::getCause() {
		return std::string(exception_getCause(this->obj));
	}

	void exception::setAction(std::string &txt) {
		exception_setAction(this->obj, txt.c_str());
	}

	std::string exception::getAction() {
		return std::string(exception_getAction(this->obj));
	}

	void exception::setComment(std::string &txt) {
		exception_setComment(this->obj, txt.c_str());
	}

	std::string exception::getComment() {
		return std::string(exception_getComment(this->obj));
	}

	void exception::show() {
		exception_show(this->obj);
	}

	bool exception::exists() {
		return exception_exists(this->obj);
	}

	void exception::reset() {
		exception_reset(this->obj);
	}
}
#endif
