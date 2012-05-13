/*
 * =====================================================================================
 *
 *       Filename:  la_error.h
 *
 *    Description:  definition to handle errors
 *
 *        Version:  1.0
 *        Created:  04/05/2012 17:29:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_ERROR_H
#define LA_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "la_common.h"

#define ERROR_MESSAGE_SIZE 1024

#define ERROR_NONE 0
#define ERROR_UNKNOWN -1

void initError();
void setError(int id, const char *msg);
int getErrorId();
char *getErrorMessage();
void showError();
BOOL isError();
void resetError();

#ifdef __cplusplus
}
#endif

#endif
