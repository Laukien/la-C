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

char *string_toLower(char *str);
char *string_toUpper(char *str);
char *string_trim(char *str);
char *string_replace_first(char *string, char *from, char *to);
char *string_replace(char *string, char *from, char *to);

#endif
