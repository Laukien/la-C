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

char *string_toLower(char *str) {
	int len = strlen(str);	

	char *result = malloc ( len );
	if ( result==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (str_tolower)\n" );
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i=0; i<len; i++ ) {
		result[i] = tolower(str[i]);
	}

	return result;
}

char *string_toUpper(char *str) {
	int len = strlen(str);	

	char *result = malloc ( len );
	if ( result==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed (str_toupper)\n" );
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i=0; i<len; i++ ) {
		result[i] = toupper(str[i]);
	}

	return result;
}


char *string_trim(char *str) {
	int idx_start=0;
	int idx_stop=strlen(str);
	while (str[idx_start] != '\0' && !isalnum(str[idx_start])) idx_start++;
	while (idx_stop>idx_start && !isalnum(str[idx_stop-1])) idx_stop--;
	if ( idx_start == idx_stop) return NULL;


	char *trim = malloc (idx_stop-idx_start+1 );
	if ( trim==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(trim, str+idx_start, idx_stop-idx_start);
	trim[idx_stop] = '\0';

	return trim;
}

char *string_replaceFirst(char *string, char *from, char *to) {
	int string_size = strlen(string);
	int from_size = strlen(from);
	int to_size = strlen(to);
	int result_size = string_size - from_size + to_size;
	char *result;

	char *begin = strstr(string, from);         /* get the first position */
	if (begin == NULL) {                        /* 'from' doesn't exists */
		result = malloc(string_size + 1);
		if (result == NULL) {
			printf ( "ERROR: Unable to get memory.\n" );
			exit(EXIT_FAILURE);
		}
		strcpy(result, string);                 /* copy  */
	} else {
		int idx = begin - string;
		result = malloc(result_size + 1);
		if (result == NULL) {
			printf ( "ERROR: Unable to get memory.\n" );
			exit(EXIT_FAILURE);
		}
		memcpy(result, string, idx);
		memcpy(result + idx, to, to_size);
		memcpy(result + idx + to_size, begin + from_size, string_size - from_size - idx);
		result[result_size] = '\0';
	}

	return result;
}

char *string_replace(char *string, char *from, char *to) {
	int string_size = strlen(string);
	int from_size = strlen(from);
	int to_size = strlen(to);
	int result_size = string_size - from_size + to_size;
	char *result;
	result = malloc(string_size + 1);
	if (result == NULL) {
		printf ( "ERROR: Unable to get memory.\n" );
		exit(EXIT_FAILURE);
	}

	char *begin = strstr(string, from);         /* get the first position */
	if (begin == NULL) {                        /* 'from' doesn't exists */
		strcpy(result, string);                 /* copy  */
	} else {
		int idx = begin - string;
		memcpy(result, string, idx);
		while (begin != NULL) {
			result_size = result_size - from_size + to_size;
			result = (char *)realloc(result, result_size + 1);
			if (result == NULL) {
				printf ( "ERROR: Unable to get memory.\n" );
				exit(EXIT_FAILURE);
			}
			memcpy(result + idx, to, to_size);
			memcpy(result + idx + to_size, begin + from_size, string_size - from_size - idx);
			begin = strstr(begin + from_size, from);
			idx = begin - string;
		}
		result[result_size] = '\0';
	}

	return result;
}

