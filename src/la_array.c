/*
 * =====================================================================================
 *
 *       Filename:  la_array.c
 *    Description:  simple string-array
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "la_message.h"
#include "la_array.h"

struct la_array {
	size_t size;
	char **array;
};

ARRAY *array_new() {
	ARRAY *self = (ARRAY*) malloc(sizeof(ARRAY));
	if (!self) {
		message_error("unable to get memory");
	}
	
	self->size = 0;
	self->array = NULL;

	return self;
}

void array_free(ARRAY *self) {
	assert(self);

	array_reset(self);
	free(self);
	self = NULL;
}

void array_reset(ARRAY *self) {
	assert(self);

	size_t i;
	for (i = 0; i < self->size; ++i) {
		if (self->array[i]) {
			free(self->array[i]);
			self->array[i] = NULL;
		}
	}

	if (self->array) {
		free(self->array);
		self->array = NULL;
	}

	self->size = 0;
}

void array_setSize(ARRAY *self, size_t size) {
	assert(self);

	if (self->size) {
		array_reset(self);
	}

	/* get memory */
	self->size = size;
	self->array = (char **)malloc((self->size + 1) * sizeof(char *));
	if (!self->array) {
		message_error("unable to get memory");
	}
	
	/* clear memory */
	size_t i;
	for (i = 0; i <= self->size; ++i) {
		self->array[i] = NULL;
	}
}

size_t array_getSize(ARRAY *self) {
	return self->size;
}

void array_show(ARRAY *self) {
	assert(self);

	unsigned int i;
	for (i = 0; i < self->size; ++i) {
		puts(self->array[i]);
	}
}

void array_setString(ARRAY *self, unsigned int idx, const char *str) {
	assert(self);
	if (idx < 0 || idx >= self->size) {
		message_error("out of bounds");
		return;
	}

	if (self->array[idx]) {
		free(self->array[idx]);
		self->array[idx] = strdup(str);
	}
}

char *array_getString(ARRAY *self, unsigned int idx) {
	assert(self);
	if (idx < 0 || idx >= self->size) return NULL;

	char *res = strdup(self->array[idx]);

	return res;
}

char **array_getArray(ARRAY *self) {
	assert(self);

	char **array = (char **)malloc((self->size + 1) * sizeof(char *));
	if (!self->array) {
		message_error("unable to get memory");
	}
	
	/* clear memory */
	size_t i;
	for (i = 0; i <= self->size; ++i) {
		if (self->array[i]) {
			array[i] = strdup(self->array[i]);
		} else {
			array[i] = (char *)NULL;
		}
	}
	array[self->size] = (char *)NULL;

	return array;
}

void array_showArray(char **array) {
	assert(array);

	size_t count = 0;
	while (array[count]) {
		puts(array[count++]);
	}
}

void array_freeArray(char **array) {
	assert(array);

	size_t count = 0;
	while(array[count]) {
		if (array[count]) {
			free(array[count]);
			array[count] = NULL;
		}
		++count;
	}

	if (array) {
		free(array);
		array = NULL;
	}
}

void array_loadFromString(ARRAY *self, const char *str, const char sep) {
	assert(self);

	array_reset(self);

	int i;
	int count;

	/* count tokens */
	count = 0;
	for (i = 0; i < strlen(str); ++i) {
		if (str[i] == sep) ++count;
	}
	++count;

	/* init memory */
	array_setSize(self, count);

	/* separate & copy tokens */
	int start = 0;
	int stop;
	count = 0;
	char *tok;
	for (i = 0; i < strlen(str) + 1; ++i) {
		if (str[i] == sep || str[i] == '\0') {
			stop = i;
			tok = (char *)malloc(stop - start + 1);
			memcpy(tok, str + start, stop - start);
			tok[stop - start] = '\0';
			self->array[count++] = strdup(tok);
			free(tok);
			start = stop + 1;
		}
	}
	self->array[count] = NULL;
}

#ifdef __cplusplus
namespace la {
	namespace vector {
		Vector::Vector() {
			this->obj = array_new();
		}

		Vector::~Vector() {
			array_free(this->obj);
		}
	}
}
#endif

