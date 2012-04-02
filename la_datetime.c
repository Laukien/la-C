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
LA_DATETIME datetime_now() {
        LA_DATETIME now;
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

	LA_DATETIME now = datetime_now();
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

	LA_DATETIME now = datetime_now();
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

	LA_DATETIME now = datetime_now();
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

	LA_DATETIME now = datetime_now();
	sprintf(date, "%02d.%02d.%04d %02d:%02d:%02d", now.monthday, now.month, now.year, now.hour, now.minute, now.second);

	return date;
}
