/*
 * =====================================================================================
 *
 *       Filename:  la_string.h
 *
 *    Description:  Header for la_stringbuffer.c
 *
 *        Version:  1.0
 *        Created:  04/09/2011 01:30:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef LA_STRINGBUFFER_H
#define LA_STRINGBUFFER_H

#include "la_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *pointer;
	unsigned int size;
} LA_STRINGBUFFER;

LA_STRINGBUFFER *stringbuffer_new();
void stringbuffer_free(LA_STRINGBUFFER *ptr);
unsigned int stringbuffer_size(LA_STRINGBUFFER *ptr);
void stringbuffer_append(LA_STRINGBUFFER *ptr, const char *str);
void stringbuffer_insert(LA_STRINGBUFFER *ptr, const char *str, unsigned int pos);
char *stringbuffer_pointer(LA_STRINGBUFFER *ptr);
LA_STRINGBUFFER *stringbuffer_clone(LA_STRINGBUFFER *ptr);
void stringbuffer_reset(LA_STRINGBUFFER *ptr);

#ifdef __cplusplus
}
#endif

#endif
