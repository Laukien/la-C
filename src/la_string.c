/*
 * =====================================================================================
 *
 *       Filename:  la_string.c
 *    Description:  common string operations
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include "la_character.h"
#include "la_string.h"

char *string_toLower(const char *string) {
	size_t len = strlen(string);	

	char *result = (char *)malloc (len + 1);
	size_t i;
	for (i = 0; i < len; ++i) {
		result[i] = tolower(string[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toUpper(const char *string) {
	size_t len = strlen(string);	

	char *result = (char *)malloc (len + 1);
	size_t i;
	for (i = 0; i <= len; ++i) {
		result[i] = toupper(string[i]);
	}
	result[len] = '\0';

	return result;
}

char *string_toCamel(const char *string) {
	size_t len = strlen(string);	

	char *tmp = (char *)malloc (len + 1);
	size_t i, j = 0;
	BOOL up = TRUE;
	for (i = 0; i <= len; ++i) {
		if (!isalnum(string[i])) {
			up = TRUE;
			continue;
		}
		if (up) {
			tmp[j] = toupper(string[i]);
			up = FALSE;
		} else {
			tmp[j] = string[i];
		}
		++j;
	}
	tmp[j] = '\0';

	char *result = strdup(tmp);
	free(tmp);

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
	if (t == NULL) return TRUE;
	size_t len = strlen(t);
	free(t);

	return len == 0 ? TRUE : FALSE;
}

BOOL string_isAlnum(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isalnum(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isAlpha(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isalpha(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isCntrl(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!iscntrl(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isDigit(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isdigit(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isGraph(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isgraph(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isLower(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!islower(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isPrint(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isprint(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isPunct(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!ispunct(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isSpace(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isspace(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isUpper(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isupper(str[i])) return FALSE;
	}

	return TRUE;
}

BOOL string_isXdigit(const char *str) {
	size_t i;
	for (i = 0; i < strlen(str); ++i) {
		if (!isxdigit(str[i])) return FALSE;
	}

	return TRUE;
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

char *string_getRandom(size_t size, BOOL upper, BOOL lower, BOOL number, BOOL special) {
	srand(time(NULL));

	int start;
	int stop;
	if (special) {
		start = 32;
		stop = 126;
	} else {
		start = 48;
		stop = 122;
	}

	char *res = (char *)malloc(size + 1);
	if (!res) {
		return NULL;
	}
	
	char c;
	size_t i;
	for (i = 0; i < size; ++i) {
		while (1) {
			c = (rand() % (stop - start)) + start;
			if (special) break;
			if (c >= '0' && c <= '9' && number) break;
			if (c >= 'A' && c <= 'Z' && upper) break;
			if (c >= 'a' && c <= 'z' && lower) break;
		}
		res[i] = c;
	}
	res[size] = '\0';

	return res;
}

#ifdef __cplusplus
namespace la {
	namespace string {
		std::string toLower(const std::string &string) {
			char *tmp = string_toLower(string.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string toUpper(const std::string &string) {
			char *tmp = string_toUpper(string.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string toCamel(const std::string &string) {
			char *tmp = string_toCamel(string.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string trim(const std::string &string) {
			char *tmp = string_trim(string.c_str());
			if (!tmp) {
				return std::string();
			}
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string replaceFirst(const std::string &string, const std::string &from, const std::string &to) {
			char *tmp = string_replaceFirst(string.c_str(), from.c_str(), to.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string replace(const std::string &string, const std::string &from, const std::string &to) {
			char *tmp = string_replace(string.c_str(), from.c_str(), to.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		list::List split(const std::string &string, const std::string &delimiters) {
			LIST *l = string_split(string.c_str(), delimiters.c_str());
			list::List res(l);

			return res;
		}

		bool isEmpty(const std::string &string) {
			return string_isEmpty(string.c_str());
		}

		bool isAlnum(const std::string &str) {
			return string_isAlnum(str.c_str());
		}
		bool isAlpha(const std::string &str) {
			return string_isAlpha(str.c_str());
		}
		bool isCntrl(const std::string &str) {
			return string_isCntrl(str.c_str());
		}
		bool isDigit(const std::string &str) {
			return string_isDigit(str.c_str());
		}
		bool isGraph(const std::string &str) {
			return string_isGraph(str.c_str());
		}
		bool isLower(const std::string &str) {
			return string_isLower(str.c_str());
		}
		bool isPrint(const std::string &str) {
			return string_isPrint(str.c_str());
		}
		bool isPunct(const std::string &str) {
			return string_isPunct(str.c_str());
		}
		bool isSpace(const std::string &str) {
			return string_isSpace(str.c_str());
		}
		bool isUpper(const std::string &str) {
			return string_isUpper(str.c_str());
		}
		bool isXDigit(const std::string &str) {
			return string_isXdigit(str.c_str());
		}

		std::string loadFromFile(const std::string &filename) {
			char *tmp = string_loadFromFile(filename.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		bool saveToFile(const std::string &filename, const std::string &string) {
			return string_saveToFile(filename.c_str(), string.c_str());
		}

	#ifdef SYSTEM_OS_TYPE_LINUX
		std::string regexp(const std::string &string, const std::string &pattern, int &begin, int &end) {
			char *tmp = string_regexp(string.c_str(), pattern.c_str(), &begin, &end);
			std::string res = tmp;
			free(tmp);

			return res;
		}
	#endif

		std::string fromHex(const std::string &string) {
			char *tmp = string_fromHex(string.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string toHex(const std::string &string) {
			char *tmp = string_toHex(string.c_str());
			std::string res = tmp;
			free(tmp);

			return res;
		}

	}

	std::string getRandom(size_t size, bool upper, bool lower, bool number, bool special) {
		char *tmp = string_getRandom(size, upper, lower, number, special);
		std::string res = tmp;
		free(tmp);

		return res;
	}

}
#endif
