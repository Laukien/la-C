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

#include "la_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _id;
char _msg[ERROR_MESSAGE_SIZE + 1];

void error_init() {
	error_reset();
}

void error_set(int id, const char *msg) {
	if (strlen(msg) > ERROR_MESSAGE_SIZE) {
		fprintf ( stderr, "ERROR: Message is longer than '%d'.\n", ERROR_MESSAGE_SIZE );
		exit(EXIT_FAILURE);
	}

	_id = id;
	strcpy(_msg, msg);
}

int error_getId() {
	return _id;
}

char *error_getMessage() {
	if (!error_exists()) return NULL;

	return _msg;
}

BOOL error_exists() {
	return (_id != ERROR_NONE || strlen(_msg) > 0);
}

void error_reset() {
	_id = ERROR_NONE;
	_msg[0] = '\0';
}

void error_show() {
	fprintf (stderr, "ERROR: %s (%d)\n", _msg, _id);
}
