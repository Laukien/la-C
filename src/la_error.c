/*
 * =====================================================================================
 *
 *       Filename:  la_error.h
 *
 *    Description:  handle errors
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "la_error.h"
#include "la_message.h"

struct la_error {
	int id;
	char *message;
	char *comment;
	char *cause;
	char *action;
};

ERROR *error_new() {
	ERROR *self = (ERROR *)malloc(sizeof(ERROR));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, '\0', sizeof(ERROR));
	self->id = ERROR_NONE;

	return self;
}

void error_free(ERROR *self) {
	assert(self);

	error_reset(self);
	free(self);
	self = NULL;
}

void error_set(ERROR *self, int id, const char *txt, ...) {
	assert(self);

	if (error_exists(self)) {
		error_reset(self);
	}

	self->id = (id != ERROR_NONE ? id : ERROR_UNKNOWN);
	if (txt) {
		va_list args;
		char buf[ERROR_MESSAGE_SIZE];
		va_start(args, txt);
		vsnprintf(buf, ERROR_MESSAGE_SIZE, txt, args);
		va_end(args);
		self->message = strdup(buf);
	}
}

void error_setId(ERROR *self, int id) {
	assert(self);

	self->id = id;
}

int error_getId(ERROR *self) {
	assert(self);

	return self->id;
}

void error_setMessage(ERROR*self, const char *txt, ...) {
	assert(self);

	/* reset */
	if (self->message) {
		free(self->message);
		self->message = NULL;
	}

	if (txt) {
		/* if no 'id' has been set, set an unknown error */
		if (self->id == ERROR_NONE) {
			self->id = ERROR_UNKNOWN;
		}

		va_list args;
		char buf[ERROR_MESSAGE_SIZE];
		va_start(args, txt);
		vsnprintf(buf, ERROR_MESSAGE_SIZE, txt, args);
		va_end(args);
		self->message = strdup(buf);
	}
}

char *error_getMessage(ERROR *self) {
	assert(self);

	return self->message;
}

void error_setComment(ERROR*self, const char *txt) {
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

char *error_getComment(ERROR *self) {
	assert(self);

	return self->comment;
}

void error_setCause(ERROR*self, const char *txt) {
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

char *error_getCause(ERROR *self) {
	assert(self);

	return self->cause;
}

void error_setAction(ERROR*self, const char *txt) {
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

char *error_getAction(ERROR *self) {
	assert(self);

	return self->action;
}

BOOL error_exists(ERROR *self) {
	assert(self);

	return self->id != ERROR_NONE;
}

void error_reset(ERROR *self) {
	assert(self);

	self->id = ERROR_NONE;
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

void error_show(ERROR *self) {
	assert(self);

	fprintf (stderr, "ERROR:\t\t%s (%d)\n", self->message, self->id);
	if (self->comment) {
		fprintf (stderr, "    COMMENT:\t%s\n", self->comment);
	}
	if (self->cause) {
		fprintf (stderr, "    CAUSE:\t%s\n", self->cause);
	}
	if (self->action) {
		fprintf (stderr, "    ACTION:\t%s\n", self->action);
	}
}

#ifdef __cplusplus
namespace la {

	error::error() {
		this->obj = error_new();
	}

	error::~error() {
		error_free(this->obj);
	}

	void error::set(int id, const std::string &txt) {
		error_set(this->obj, id, txt.c_str());
	}

	void error::setId(int id) {
		error_setId(this->obj, id);
	}

	int error::getId() {
		return error_getId(this->obj);
	}

	void error::setMessage(std::string &txt) {
		error_setMessage(this->obj, txt.c_str());
	}

	std::string error::getMessage() {
		return std::string(error_getMessage(this->obj));
	}

	void error::setComment(std::string &txt) {
		error_setComment(this->obj, txt.c_str());
	}

	std::string error::getComment() {
		return std::string(error_getComment(this->obj));
	}

	void error::setCause(std::string &txt) {
		error_setCause(this->obj, txt.c_str());
	}

	std::string error::getCause() {
		return std::string(error_getCause(this->obj));
	}

	void error::setAction(std::string &txt) {
		error_setAction(this->obj, txt.c_str());
	}

	std::string error::getAction() {
		return std::string(error_getAction(this->obj));
	}

	void error::show() {
		error_show(this->obj);
	}

	bool error::exists() {
		return error_exists(this->obj);
	}

	void error::reset() {
		error_reset(this->obj);
	}
}
#endif
