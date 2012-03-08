/*
 * =====================================================================================
 *
 *       Filename:  la_datetime.h
 *
 *    Description:  header for la_datetime.c
 *
 *        Version:  1.0
 *        Created:  10/11/2011 01:20:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_DATETIME_H
#define LA_DATETIME_H

typedef struct {
        short year;
        short month;
        short monthday;
        short weekday;
        short hour;
        short minute;
        short second;
} DATETIME;

DATETIME datetime_now();
char *datetime_getTimestampAsString();

#endif
