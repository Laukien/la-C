/*
 * =====================================================================================
 *
 *       Filename:  la_datetime.h
 *    Description:  date & time functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_DATETIME_H
#define LA_DATETIME_H

#ifdef __cplusplus
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
} DATETIME;

DATETIME datetime_now();
char *datetime_getTimestampAsString();
char *datetime_getDateAsString();
char *datetime_getTimeAsString();
char *datetime_getDateTimeAsString();

#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace datetime {
		std::string getTimestampAsString();
		std::string getDateAsString();
		std::string getTimeAsString();
		std::string getDateTimeAsString();
	}
}
#endif

#endif
