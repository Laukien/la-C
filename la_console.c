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
#include <termios.h>

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

int console_getKey(int byte, int second) {
    struct termios orig_termios;                /* terminal structure */
    volatile int ttyfd = STDIN_FILENO;          /* STDIN (0) */

	/* save old state */
    if (tcgetattr(ttyfd,&orig_termios) < 0) return -1;

	/* switch to raw */
    struct termios raw;
    raw = orig_termios;                         /* copy original and then modify below */
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = byte; raw.c_cc[VTIME] = second;

    if (tcsetattr(ttyfd,TCSAFLUSH,&raw) < 0) return -1;

	/* read key form stdin */
	int n;
	int c;
	n = read(ttyfd, &c, 1);
	if (n < 0) c = -1;
	else if (n == 0) c = 0;

	/* reset state */
    if (tcsetattr(ttyfd,TCSAFLUSH,&orig_termios) < 0) return -1;

	return c;
}
