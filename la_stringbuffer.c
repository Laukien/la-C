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

LA_STRINGBUFFER *stringbuffer_new() {
	LA_STRINGBUFFER *ptr = (LA_STRINGBUFFER *)malloc(sizeof(LA_STRINGBUFFER));
	if (ptr == NULL) {
		printf ( "ERROR: Unable to get memory.\n" );
		exit(EXIT_FAILURE);
	}

	ptr->pointer = (char *)malloc(1);
	ptr->pointer[0] = '\0';
	ptr->size = 0;

	return ptr;
}

void stringbuffer_free(LA_STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	free(ptr->pointer);
	ptr->size = 0;
	free(ptr);
}

unsigned int stringbuffer_size(LA_STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	return ptr->size;
}

void stringbuffer_append(LA_STRINGBUFFER *ptr, const char *str) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

    ptr->size = ptr->size + strlen(str);        /* set new length */
	ptr->pointer = (char *) realloc(ptr->pointer, ptr->size + 1);
	if (ptr->pointer == NULL) {
		printf ( "ERROR: Unable to get more memory.\n" );
		exit(EXIT_FAILURE);
	}
	memcpy(ptr->pointer+strlen(ptr->pointer), str, strlen(str)); 
	ptr->pointer[ptr->size] = '\0';             /* end string */
}

void stringbuffer_insert(LA_STRINGBUFFER *ptr, const char *str, unsigned int pos) {
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
		memcpy(tmp, ptr->pointer, pos); 
	}
	memcpy(tmp + pos, str, strlen(str));
	if(pos != strlen(ptr->pointer)) {
		memcpy(tmp + pos + strlen(str), ptr->pointer + pos, strlen(ptr->pointer) - pos);
	}

	free(ptr->pointer);
	ptr->pointer = tmp;
}

char *stringbuffer_pointer(LA_STRINGBUFFER *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	return ptr->pointer;
}

LA_STRINGBUFFER *stringbuffer_clone(LA_STRINGBUFFER *ptr) {
	LA_STRINGBUFFER *new_ptr = stringbuffer_new();
	stringbuffer_append(new_ptr, ptr->pointer);

	return new_ptr;
}

void stringbuffer_reset(LA_STRINGBUFFER *ptr) {
	free(ptr->pointer);
	ptr->pointer = (char *)malloc(1);
	if ( ptr->pointer == NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (stringbuffer_reset)\n" );
		exit (EXIT_FAILURE);
	}
	ptr->size = 0;
}
