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

#include "la_exception.h"
#include <stdlib.h>
#include <stdio.h>

EXCEPTION *exception_new() {
	EXCEPTION *e = (EXCEPTION *)malloc(sizeof(EXCEPTION));
	e->level = 0;

	return e;
}

void exception_addCallback(EXCEPTION *e, EXCEPTION_CALLBACK callback, void *pointer) {
	if (e->level >= EXCEPTION_CALLBACK_SIZE) {
		fprintf(stderr, "ERROR - Exception: callback-level to deep\n");
		exit(EXIT_FAILURE);
	}

	e->callback[e->level] = callback;
	e->pointer[e->level] = pointer;
	++(e->level);
}

void exception_delCallback(EXCEPTION *e) {
	if (e->level <= 0) {
		fprintf(stderr, "ERROR - Exception: no callback to delete\n");
		exit(EXIT_FAILURE);
	}

	--(e->level);
}

void exception_throw(EXCEPTION *e, int id, char *message) {
	int i;
	for (i = e->level - 1; i >= 0; --i) {
		e->callback[i](id, message, e->pointer[i]);
	}
}

int exception_level(EXCEPTION *e) {
	return e->level;
}

BOOL exception_exists(EXCEPTION *e) {
	return e->level ? TRUE : FALSE;
}

void exception_free(EXCEPTION *e) {
	if (e != NULL) {
		free(e);
		e = NULL;
	}
}

