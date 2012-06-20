/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *
 *    Description:  enhanced string-functions
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
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	/* clone text */
	char *text = (char *)malloc(strlen(ptr->text + 1));
	if (text == NULL) {
		printf ( "ERROR: Unable to get more memory.\n" );
		exit(EXIT_FAILURE);
	}
	strcpy(text, ptr->text);

	return text;
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
