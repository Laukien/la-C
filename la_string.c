/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *
 *    Description:  common string-functions
 *
 *        Version:  1.0
 *        Created:  11/07/2011 01:10:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */

#include "la_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

LA_STRING *string_new() {
	LA_STRING *ptr = (LA_STRING *)malloc(sizeof(LA_STRING));
	if (ptr == NULL) {
		printf ( "ERROR: Unable to get memory.\n" );
		exit(EXIT_FAILURE);
	}

	ptr->pointer = (char *)malloc(1);
	ptr->pointer[0] = '\0';
	ptr->size = 0;

	return ptr;
}

void string_free(LA_STRING *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	free(ptr->pointer);
	ptr->size = 0;
	free(ptr);
}

unsigned int string_size(LA_STRING *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	return ptr->size;
}

void string_append(LA_STRING *ptr, const char *str) {
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

void string_insert(LA_STRING *ptr, const char *str, unsigned int pos) {
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

char *string_pointer(LA_STRING *ptr) {
	if (ptr == NULL) {
		printf ( "ERROR: Pointer is NULL.\n" );
		exit(EXIT_FAILURE);
	}

	return ptr->pointer;
}

LA_STRING *string_clone(LA_STRING *ptr) {
	LA_STRING *new_ptr = string_new();
	string_append(new_ptr, ptr->pointer);

	return new_ptr;
}

void string_reset(LA_STRING *ptr) {
	free(ptr->pointer);
	ptr->pointer = (char *)malloc(1);
	if ( ptr->pointer == NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_reset)\n" );
		exit (EXIT_FAILURE);
	}
	ptr->size = 0;
}

char *string_toLower(const char *str) {
	int len = strlen(str);	

	char *result = (char *)malloc ( len + 1);
	if ( result==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_toLower)\n" );
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i = 0; i < len; ++i ) {
		result[i] = tolower(str[i]);
	}

	return result;
}

char *string_toUpper(const char *str) {
	int len = strlen(str);	

	char *result = (char *)malloc ( len + 1 );
	if ( result==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_toUpper)\n" );
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i = 0; i <= len; ++i ) {
		result[i] = toupper(str[i]);
	}

	return result;
}


char *string_trim(const char *str) {
	int idx_start=0;
	int idx_stop=strlen(str);
	while (str[idx_start] != '\0' && iscntrl(str[idx_start])) idx_start++;
	while (idx_stop>idx_start && iscntrl(str[idx_stop-1])) idx_stop--;
	if ( idx_start == idx_stop) return NULL;


	char *trim = (char *)malloc (idx_stop-idx_start+1 );
	if ( trim==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_trim)\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(trim, str+idx_start, idx_stop-idx_start);
	trim[idx_stop] = '\0';

	return trim;
}

char *string_replaceFirst(const char *string, const char *from, const char *to) {
	int string_size = strlen(string);
	int from_size = strlen(from);
	int to_size = strlen(to);
	int result_size = string_size - from_size + to_size;
	char *result;

    const char *begin = strstr(string, from);   /* get the first position */
	if (begin == NULL) {                        /* 'from' doesn't exists */
		result = (char *)malloc(string_size + 1);
		if (result == NULL) {
			fprintf ( stderr, "\ndynamic memory allocation failed (string_replaceFirst)\n" );
			exit(EXIT_FAILURE);
		}
		strcpy(result, string);                 /* copy  */
	} else {
		int idx = begin - string;
		result = (char *)malloc(result_size + 1);
		if (result == NULL) {
			fprintf ( stderr, "\ndynamic memory allocation failed (string_replaceFirst)\n" );
			exit(EXIT_FAILURE);
		}
		memcpy(result, string, idx);
		memcpy(result + idx, to, to_size);
		memcpy(result + idx + to_size, begin + from_size, string_size - from_size - idx);
		result[result_size] = '\0';
	}

	return result;
}

char *string_replace(const char *string, const char *from, const char *to) {
	int string_size = strlen(string);
	int from_size = strlen(from);
	int to_size = strlen(to);
	int result_size = string_size;
	char *result;
	result = (char *)malloc(string_size + 1);
	if (result == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_replace)\n" );
		exit(EXIT_FAILURE);
	}

    const char *begin = strstr(string, from);   /* get the first position */
	if (begin == NULL) {                        /* 'from' doesn't exists */
		strcpy(result, string);                 /* copy  */
	} else {
		int count = 0;
		int idx = begin - string;
		memcpy(result, string, idx);
		while (begin != NULL) {
			result_size = result_size - from_size + to_size;
			result = (char *)realloc(result, result_size + 1);
			if (result == NULL) {
				fprintf ( stderr, "\ndynamic memory allocation failed (string_replace)\n" );
				exit(EXIT_FAILURE);
			}
//			memset(result, '\0', result_size);
			memcpy(result + idx, to, to_size);
			memcpy(result + idx + to_size, begin + from_size, string_size - idx+(count * (to_size - from_size)));
			begin = strstr(begin + from_size, from);
			++count;
			idx = begin - string + (count * (to_size - from_size));
		}
		result[result_size] = '\0';
	}

	return result;
}

BOOL string_isEmpty(const char *string) {
	if (string == NULL) return TRUE;
	char *t = string_trim(string);
	size_t len = strlen(t);
	free(t);

	return len == 0 ? TRUE : FALSE;
}
