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

char *string_toLower(const char *string) {
	int len = strlen(string);	

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
		result[i] = tolower(string[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toUpper(const char *string) {
	int len = strlen(string);	

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
		result[i] = toupper(string[i]);
	}
	result[len] = '\0';

	return result;
}


char *string_trim(const char *string) {
	if (string == NULL) return NULL;
	int idx_start = 0;
	int idx_stop = strlen(string);
		
	while (string[idx_start] != '\0' && (iscntrl(string[idx_start]) || string[idx_start] == ' ')) idx_start++;
	while (idx_stop > idx_start && (iscntrl(string[idx_stop - 1]) || string[idx_stop -1] == ' ')) idx_stop--;
	if (idx_start == idx_stop) return NULL;


	char *trim = (char *)malloc (idx_stop - idx_start + 1 );
	if (trim == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_trim)\n" );
		exit (EXIT_FAILURE);
	}

	memcpy(trim, string + idx_start, idx_stop - idx_start);
	trim[idx_stop - idx_start] = '\0';

	return trim;
}

LIST *string_split(const char *string, const char *delimiters) {
	LIST *list = list_new();
	const char *s = string;
	const char *e = s;
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

//char *string_replace(const char *string, const char *from, const char *to) {
//	int string_size = strlen(string);
//	int from_size = strlen(from);
//	int to_size = strlen(to);
//	int result_size = string_size;
//	int diff_size;
//	char *result;
//	result = (char *)malloc(string_size + 1);
//	if (result == NULL) {
//		fprintf ( stderr, "\ndynamic memory allocation failed (string_replace)\n" );
//		exit(EXIT_FAILURE);
//	}
//
//    const char *begin = strstr(string, from);   /* get the first position */
//	if (begin == NULL) {                        /* 'from' doesn't exists */
//		strcpy(result, string);                 /* copy  */
//	} else {
//		int count = 0;
//		int idx = begin - string;
//		memcpy(result, string, idx);
//		while (begin != NULL) {
//			/* calculate absulte diffence between 'to' and 'from' */
//			if (from_size == to_size)
//				diff_size = 0;
//			else if (from_size > to_size)
//				diff_size = to_size - from_size;
//			else if (from_size < to_size)
//				diff_size = from_size - to_size;
//
//			/* get memory an its size */
//			result_size = result_size - diff_size;
//			result = (char *)realloc(result, result_size + 1);
//			if (result == NULL) {
//				fprintf ( stderr, "\ndynamic memory allocation failed (string_replace)\n" );
//				exit(EXIT_FAILURE);
//			}
//
//			/* copy 'to' to result */
//			memcpy(result + idx, to, to_size);
//			memcpy(result + idx + to_size, begin + from_size, string_size - idx);
//
//			/* get next 'from' */
//			begin = strstr(begin + from_size, from);
//			++count;
//			idx = begin - string + (count * diff_size);
//		}
//		result[result_size] = '\0';
//	}
//
//	return result;
//}

char *string_replace(const char *string, const char *from, const char *to) {
	char *result;
	char *ins;
	char *tmp;
	int len_from;
	int len_to;
	int len_front;
	int count;

	/* check parameter */
	if (!string)
		return NULL;
	if (!from || !(len_from = strlen(from)))
		return NULL;
	if (!(ins = strstr((char*)string, from)))   /* "(char *) - "g++ bug */
		return strdup(string);
	if (!to || !(len_to = strlen(to)))
		return NULL;

	/* count hits */
	for (count = 0; (tmp = strstr(ins, from)) != NULL; ++count) {
		ins = tmp + len_from;
	}

	/* get memory */
	tmp = result = (char *) malloc(strlen(string) + (len_to - len_from) * count + 1);
	if (result == NULL) {
		fprintf ( stderr, "\ndynamic memory allocation failed (string_replace)\n" );
		exit(EXIT_FAILURE);
	}

	while (count--) {
		ins = strstr((char *)string, from);
		len_front = ins - string;
		tmp = strncpy(tmp, string, len_front) + len_front;
		tmp = strcpy(tmp, to) + len_to;
		string += len_front + len_from;
	}
	strcpy(tmp, string);

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

	fclose(file);

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
char *string_regexp (const char *string, const char *pattern, int *begin, int *end) {     
	int i, len;                  
	int w = 0;
	char *word = NULL;
	regex_t rgT;
	regmatch_t match;
	regcomp(&rgT, pattern, REG_EXTENDED);
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

#ifdef __cplusplus
namespace la {
	namespace string {
		std::string toLower(const std::string &string) {
			return std::string(string_toLower(string.c_str()));
		}

		std::string toUpper(const std::string &string) {
			return std::string(string_toUpper(string.c_str()));
		}

		std::string trim(const std::string &string) {
			return std::string(string_trim(string.c_str()));
		}

		std::string replaceFirst(const std::string &string, const std::string &from, const std::string &to) {
			return std::string(string_replaceFirst(string.c_str(), from.c_str(), to.c_str()));
		}

		std::string replace(const std::string &string, const std::string &from, const std::string &to) {
			return std::string(string_replace(string.c_str(), from.c_str(), to.c_str()));
		}

		list split(const std::string &string, const std::string &delimiters) {
			LIST *l = string_split(string.c_str(), delimiters.c_str());
			list res(l);

			return res;
		}

		bool isEmpty(const std::string &string) {
			return string_isEmpty(string.c_str());
		}

		std::string loadFromFile(const std::string &filename) {
			return std::string(string_loadFromFile(filename.c_str()));
		}

		bool saveToFile(const std::string &filename, const std::string &string) {
			return string_saveToFile(filename.c_str(), string.c_str());
		}

	#ifdef SYSTEM_OS_TYPE_LINUX
		std::string regexp(const std::string &string, const std::string &pattern, int &begin, int &end) {
			return std::string(string_regexp(string.c_str(), pattern.c_str(), &begin, &end));
		}
	#endif

		std::string fromHex(const std::string &string) {
			return std::string(string_fromHex(string.c_str()));
		}

		std::string toHex(const std::string &string) {
			return std::string(string_toHex(string.c_str()));
		}

	}
}
#endif
