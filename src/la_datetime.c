/*
 * =====================================================================================
 *
 *       Filename:  la_datetime.h
 *
 *    Description:  common date-/time-functions
 *
 *        Version:  1.0
 *        Created:  10/11/2011 01:10:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien
 *        Company:  
 *
 * =====================================================================================
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "la_datetime.h"
DATETIME datetime_now() {
        DATETIME now;
        time_t ltime;
        struct tm *Tm;

        ltime=time(NULL);
        Tm=localtime(&ltime);

        now.year=Tm->tm_year+1900;
        now.month=Tm->tm_mon+1;
        now.monthday=Tm->tm_mday;
        now.weekday=Tm->tm_wday;
        now.hour=Tm->tm_hour;
        now.minute=Tm->tm_min;
        now.second=Tm->tm_sec;

        return now;
}

char *datetime_getTimestampAsString() {

	char *date;
	date = (char *) malloc (15);
	if ( date==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	DATETIME now = datetime_now();
	sprintf(date, "%4d%02d%02d%02d%02d%02d", now.year, now.month, now.monthday, now.hour, now.minute, now.second);

	return date;
}

char *datetime_getDateAsString() {

	char *date;
	date = (char *) malloc (11);
	if ( date==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	DATETIME now = datetime_now();
	sprintf(date, "%02d.%02d.%04d", now.monthday, now.month, now.year);

	return date;
}

char *datetime_getTimeAsString() {

	char *date;
	date = (char *) malloc (9);
	if ( date==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	DATETIME now = datetime_now();
	sprintf(date, "%02d:%02d:%02d", now.hour, now.minute, now.second);

	return date;
}

char *datetime_getDateTimeAsString() {

	char *date;
	date = (char *) malloc (20);
	if ( date==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	DATETIME now = datetime_now();
	sprintf(date, "%02d.%02d.%04d %02d:%02d:%02d", now.monthday, now.month, now.year, now.hour, now.minute, now.second);

	return date;
}

#ifdef __cplusplus
namespace la {
	namespace datetime {
		std::string getTimestampAsString() {
			char *ts = datetime_getTimestampAsString();
			std::string tss = ts;
			free(ts);

			return tss;
		}

		std::string getDateAsString() {
			char *ts = datetime_getDateAsString();
			std::string tss = ts;
			free(ts);

			return tss;
		}

		std::string getTimeAsString() {
			char *ts = datetime_getTimeAsString();
			std::string tss = ts;
			free(ts);

			return tss;
		}
		std::string getDateTimeAsString() {
			char *ts = datetime_getDateTimeAsString();
			std::string tss = ts;
			free(ts);

			return tss;
		}
	}
}
#endif

