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

typedef struct {
	char *pointer;
	unsigned int size;
} LA_STRING;

LA_STRING *string_new();
void string_free(LA_STRING *ptr);
unsigned int string_size(LA_STRING *ptr);
void string_append(LA_STRING *ptr, char *str);
void string_insert(LA_STRING *ptr, char *str, unsigned int pos);
char *string_pointer(LA_STRING *ptr);

char *string_toLower(char *str);
char *string_toUpper(char *str);
char *string_trim(char *str);
char *string_replaceFirst(char *string, char *from, char *to);
char *string_replace(char *string, char *from, char *to);

#endif
