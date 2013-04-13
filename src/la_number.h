/*
 * =====================================================================================
 *
 *       Filename:  la_number.h
 *
 *    Description:  convert numbers
 *
 *        Version:  1.0
 *        Created:  03/09/2012 07:29:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_NUMBER_H
#define LA_NUMBER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

size_t number_intToSize(int num);
size_t number_uintToSize(unsigned int num);
char *number_intToString(int num);
char *number_uintToString(unsigned int num);

#ifdef __cplusplus
}
#endif

#endif
