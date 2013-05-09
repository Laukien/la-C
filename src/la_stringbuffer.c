/*
 * =====================================================================================
 *
 *       Filename:  la_stringbuffer.h
 *
 *    Description:  enhanced string-concat-functions
 *
 *        Version:  1.0
 *        Created:  04/09/2011 01:30:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */

#include "la_stringbuffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct la_stringbuffer {
	char *text;
	unsigned int size;
};

STRINGBUFFER *stringbuffer_new() {
	STRINGBUFFER *ptr = (STRINGBUFFER *)malloc(sizeof(STRINGBUFFER));
	if (ptr == NULL) {
		printf ( "ERROR: Unable to get memory.\n" );
		exit(EXIT_FAILURE);
	}

	ptr->text = (char *)malloc(1);
	ptr->text[0] = '\0';
	ptr->size = 0;

	return ptr;
}

void stringbuffer_free(STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	free(ptr->text);
	ptr->size = 0;
	free(ptr);
}

unsigned int stringbuffer_size(STRINGBUFFER *ptr) {
	return stringbuffer_getLength(ptr);
}

unsigned int stringbuffer_getLength(STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	return ptr->size;
}

void stringbuffer_append(STRINGBUFFER *ptr, const char *str) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

    ptr->size = ptr->size + strlen(str);        /* set new length */
	ptr->text = (char *) realloc(ptr->text, ptr->size + 1);
	if (ptr->text == NULL) {
		printf ( "ERROR: Unable to get more memory.\n" );
		exit(EXIT_FAILURE);
	}
	memcpy(ptr->text+strlen(ptr->text), str, strlen(str)); 
	ptr->text[ptr->size] = '\0';             /* end string */
}

void stringbuffer_insert(STRINGBUFFER *ptr, const char *str, unsigned int pos) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

    ptr->size = ptr->size + strlen(str);        /* set new length */
	char *tmp = (char*) malloc(ptr->size + 1);
	if (tmp == NULL) {
		printf ( "ERROR: Unable to get more memory.\n" );
		exit(EXIT_FAILURE);
	}
	if (pos != 0) {
		memcpy(tmp, ptr->text, pos); 
	}
	memcpy(tmp + pos, str, strlen(str));
	if(pos != strlen(ptr->text)) {
		memcpy(tmp + pos + strlen(str), ptr->text + pos, strlen(ptr->text) - pos);
	}

	free(ptr->text);
	ptr->text = tmp;
}

char *stringbuffer_text(STRINGBUFFER *ptr) {
	return stringbuffer_getText(ptr);
}

char *stringbuffer_getText(STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	/* clone text */
	char *text = (char *)malloc(strlen(ptr->text) + 1);
	if (text == NULL) {
		printf ( "ERROR: Unable to get more memory.\n" );
		exit(EXIT_FAILURE);
	}
	strcpy(text, ptr->text);

	return text;
}

char *stringbuffer_getTextPointer(STRINGBUFFER *ptr) {
	return ptr->text;
}

STRINGBUFFER *stringbuffer_clone(STRINGBUFFER *ptr) {
	STRINGBUFFER *new_ptr = stringbuffer_new();
	stringbuffer_append(new_ptr, ptr->text);

	return new_ptr;
}

void stringbuffer_reset(STRINGBUFFER *ptr) {
	free(ptr->text);
	ptr->text = (char *)malloc(1);
	if ( ptr->text == NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (stringbuffer_reset)\n" );
		exit (EXIT_FAILURE);
	}
	ptr->size = 0;
}

#ifdef __cplusplus
namespace la {
	stringbuffer::stringbuffer() {
		this->obj = stringbuffer_new();
	}

	stringbuffer::stringbuffer(STRINGBUFFER *obj) {
		this->obj = obj;
	}

	stringbuffer::~stringbuffer() {
		stringbuffer_free(this->obj);
	}

	unsigned int stringbuffer::getLength() {
		return stringbuffer_getLength(this->obj);
	}

	void stringbuffer::append(const std::string &string) {
		stringbuffer_append(this->obj, string.c_str());
	}

	void stringbuffer::insert(const std::string &string, unsigned int pos) {
		stringbuffer_insert(this->obj, string.c_str(), pos);
	}

	std::string stringbuffer::getText() {
		char *tmp = stringbuffer_getText(this->obj);
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	stringbuffer stringbuffer::clone() {
		stringbuffer sb(stringbuffer_clone(this->obj));

		return sb;
	}

	void stringbuffer::reset() {
		stringbuffer_reset(this->obj);
	}
}
#endif
