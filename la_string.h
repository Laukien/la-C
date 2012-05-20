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

#include "la_common.h"
#include <regex.h>

#ifdef __cplusplus
extern "C" {
#endif

char *string_toLower(const char *str);
char *string_toUpper(const char *str);
char *string_trim(const char *str);
char *string_replaceFirst(const char *string, const char *from, const char *to);
char *string_replace(const char *string, const char *from, const char *to);
BOOL string_isEmpty(const char *string);
char *regexp (char *string, char *patrn, int *begin, int *end);

#ifdef __cplusplus
}
#endif

#endif
