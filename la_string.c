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

#include "la_character.h"
#include "la_string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

char *string_toLower(const char *str) {
	int len = strlen(str);	

	char *result = (char *)malloc (len + 1);
	if ( result==NULL ) {
#ifdef SYSTEM_OS_TYPE_UNIX
		fprintf ( stderr, "\ndynamic memory allocation failed (string_toLower)\n" );
#else
		fprintf ( stdout, "\ndynamic memory allocation failed (string_toLower)\n" );
#endif
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i = 0; i < len; ++i ) {
		result[i] = tolower(str[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toUpper(const char *str) {
	int len = strlen(str);	

	char *result = (char *)malloc (len + 1);
	if ( result==NULL ) {
#ifdef SYSTEM_OS_TYPE_UNIX
		fprintf ( stderr, "\ndynamic memory allocation failed (string_toUpper)\n" );
#else
		fprintf ( stdout, "\ndynamic memory allocation failed (string_toUpper)\n" );
#endif
		exit (EXIT_FAILURE);
	}

	int i;
	for ( i = 0; i <= len; ++i ) {
		result[i] = toupper(str[i]);
	}
	result[len] = '\0';

	return result;
}


char *string_trim(const char *str) {
	int idx_start = 0;
	int idx_stop=strlen(str);
	while (str[idx_start] != '\0' && iscntrl(str[idx_start])) idx_start++;
	while (idx_stop>idx_start && iscntrl(str[idx_stop - 1])) idx_stop--;
	if (idx_start == idx_stop) return NULL;


	char *trim = (char *)malloc (idx_stop-idx_start + 1 );
	if (trim == NULL) {
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

LIST *string_split(const char *string, const char *delimiters) {
	LIST *list = list_new();
	const char* s = string;
	const char* e = s;
	size_t len;
	char *token;

	while (*e != 0) {
		e = s;
		while (*e != 0 && strchr(delimiters, *e) == 0) ++e;
		if (e - s > 0) {
			len = e - s;
			token = (char *) malloc(len + 1);

			memcpy(token, s, len);
			token[len] = '\0';                  /* finish string */

			list_add(list, token);              /* add token to list */

			free(token);
		}
		s = e + 1;
	}

	return list;
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

char *string_loadFromFile(const char *filename) {
	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL) return NULL;

	char line[512 + 1];
	char *str = (char *) malloc(1024 * 10);     /* get 10KB */
	str[0] = '\0';                              /* empty string */
	while (fgets(line, 512, file) != NULL) {
		str = (char *) realloc(str, strlen(str) + strlen(line) + 1);
		strcat(str, line);
	}

	return str;
}

BOOL string_saveToFile(const char *filename, const char *str) {
	FILE *file;
	file = fopen(filename, "w");
	if (file == NULL) return FALSE;

	fputs(str, file);

	fclose(file);

	return TRUE;
}

#ifdef SYSTEM_OS_TYPE_LINUX
char *string_regexp (char *string, char *patrn, int *begin, int *end) {     
	int i, len;                  
	int w = 0;
	char *word = NULL;
	regex_t rgT;
	regmatch_t match;
	regcomp(&rgT, patrn, REG_EXTENDED);
	if ((regexec(&rgT, string, 1, &match, 0)) == 0) {
		*begin = (int)match.rm_so;
		*end = (int)match.rm_eo;
		len = *end-*begin;
		word = (char*)malloc(len + 1);
		for (i = *begin; i < *end; ++i) {
			word[w] = string[i];
			++w;
		}
		word[w] = '\0';
	}
	regfree(&rgT);
	return word;
}
#endif

char *string_fromHex(const char *str) {
	unsigned int len = strlen(str) / 2;	
	assert (len * 2 == strlen(str));

	char *result = (char *)malloc (len + 1);
	if ( result==NULL ) {
#ifdef SYSTEM_OS_TYPE_UNIX
		fprintf ( stderr, "\ndynamic memory allocation failed (string_fromHex)\n" );
#else
		fprintf ( stdout, "\ndynamic memory allocation failed (string_fromHex)\n" );
#endif
		exit (EXIT_FAILURE);
	}

	unsigned int i;
	for (i = 0; i < len; ++i) {
		result[i] = character_fromHex(str[i * 2]) << 4 | character_fromHex(str[i * 2 + 1]);
	}

	result[len] = '\0';

	return result;
}

char *string_toHex(const char *str) {
	unsigned int len = strlen(str);	

	char *result = (char *)malloc ((len * 2) + 1);
	if ( result==NULL ) {
#ifdef SYSTEM_OS_TYPE_UNIX
		fprintf ( stderr, "\ndynamic memory allocation failed (string_toHex)\n" );
#else
		fprintf ( stdout, "\ndynamic memory allocation failed (string_toHex)\n" );
#endif
		exit (EXIT_FAILURE);
	}

	unsigned int i;
	for (i = 0; i < len; ++i) {
		result[i * 2] = character_toHex(str[i] >> 4);
		result[i * 2 + 1] = character_toHex(str[i] & 15);
	}
	result [len * 2] = '\0';

	return result;
}

