/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *
 *    Description:  Header for la_string.c
 *
 *        Version:  1.0
 *        Created:  11/07/2011 01:20:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef LA_STRING_H
#define LA_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *pointer;
	unsigned int size;
} LA_STRING;

LA_STRING *string_new();
void string_free(LA_STRING *ptr);
unsigned int string_size(LA_STRING *ptr);
void string_append(LA_STRING *ptr, const char *str);
void string_insert(LA_STRING *ptr, const char *str, unsigned int pos);
char *string_pointer(LA_STRING *ptr);
LA_STRING *string_clone(LA_STRING *ptr);
void string_reset(LA_STRING *ptr);

char *string_toLower(const char *str);
char *string_toUpper(const char *str);
char *string_trim(const char *str);
char *string_replaceFirst(const char *string, const char *from, const char *to);
char *string_replace(const char *string, const char *from, const char *to);

#ifdef __cplusplus
}
#endif

#endif
