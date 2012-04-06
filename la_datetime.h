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

#ifdef __cplusplus
#include <string>
namespace datetime {
	std::string getTimestampAsString();
	std::string getDateAsString();
	std::string getTimeAsString();
	std::string getDateTimeAsString();
}
extern "C" {
#endif

typedef struct {
        short year;
        short month;
        short monthday;
        short weekday;
        short hour;
        short minute;
        short second;
} LA_DATETIME;

LA_DATETIME datetime_now();
char *datetime_getTimestampAsString();
char *datetime_getDateAsString();
char *datetime_getTimeAsString();
char *datetime_getDateTimeAsString();

#ifdef __cplusplus
}
#endif

#endif
