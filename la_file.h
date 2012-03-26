/*
 * =====================================================================================
 *
 *       Filename:  la_file.h
 *
 *    Description:  header for la_file.c
 *
 *        Version:  1.0
 *        Created:  11/11/2011 02:26:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_FILE_H
#define LA_FILE_H

#include "la_common.h"

BOOL file_exists(char *filename);
char *file_name(char *file);

#endif
