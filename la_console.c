/*
 * =====================================================================================
 *
 *       Filename:  la_console.h
 *
 *    Description:  basic functions to control the console
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

#include "la_console.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void console_clean() {
	printf ( CONSOLE_CLEAN );
}

int console_getRow() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_row;
}

int console_getColumn() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_col;
}

int console_getWidth() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_xpixel;
}

int console_getHeight() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_ypixel;
}
