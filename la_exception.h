/*
 * =====================================================================================
 *
 *       Filename:  exception.h
 *
 *    Description:  Exeption handeling
 *
 *        Version:  1.0
 *        Created:  07/01/2012 06:40:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_EXCEPTION_H
#define LA_EXCEPTION_H

#include "la_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXCEPTION_CALLBACK_SIZE 16

typedef void (*EXCEPTION_CALLBACK)(int id, char *message, void *pointer);
typedef struct {
	int level;
	EXCEPTION_CALLBACK callback[EXCEPTION_CALLBACK_SIZE];
	void *pointer[EXCEPTION_CALLBACK_SIZE];
} EXCEPTION;

EXCEPTION *exception_new();
void exception_addCallback(EXCEPTION *e, EXCEPTION_CALLBACK callback, void *pointer);
void exception_delCallback(EXCEPTION *e);
void exception_throw(EXCEPTION *e, int id, char *message);
int exception_level(EXCEPTION *e);
BOOL exception_exists(EXCEPTION *e);
void exception_free(EXCEPTION *e);

#ifdef __cplusplus
}
#endif

#endif
