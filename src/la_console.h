/*
 * =====================================================================================
 *
 *       Filename:  la_console.h
 *    Description:  basic console commands
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_CONSOLE_H
#define LA_CONSOLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* console: common */
#define CONSOLE_DEFAULT			"\033[0m"
#define CONSOLE_UNDERLINE		"\033[4m"
#define CONSOLE_BLINK			"\033[5m"
#define CONSOLE_INVERSE			"\033[7m"
#define CONSOLE_CONCEALED		"\033[8m"
#define CONSOLE_CLEAN			"\033[H\033[J"

/* console: foreground */
#define CONSOLE_FG_BLACK		"\033[0;30m"
#define CONSOLE_FG_BLUE			"\033[0;34m"
#define CONSOLE_FG_GREEN		"\033[0;32m"
#define CONSOLE_FG_CYAN			"\033[0;36m"
#define CONSOLE_FG_RED			"\033[0;31m"
#define CONSOLE_FG_PURPLE		"\033[0;35m"
#define CONSOLE_FG_YELLOW		"\033[0;33m"
#define CONSOLE_FG_LIGHT_GREY	"\033[0;37m"
#define CONSOLE_FG_DARK_GREY	"\033[1;30m"
#define CONSOLE_FG_LIGHT_BLUE	"\033[1;34m"
#define CONSOLE_FG_LIGHT_GREEN	"\033[1;32m"
#define CONSOLE_FG_LIGHT_CYAN	"\033[1;36m"
#define CONSOLE_FG_LIGHT_RED	"\033[1;31m"
#define CONSOLE_FG_LIGHT_PURPLE	"\033[1;35m"
#define CONSOLE_FG_LIGHT_YELLOW	"\033[1;33m"
#define CONSOLE_FG_WHITE		"\033[1;37m"

/* console: background */
#define CONSOLE_BG_BLACK		"\033[0;40m"
#define CONSOLE_BG_BLUE			"\033[0;44m"
#define CONSOLE_BG_GREEN		"\033[0;42m"
#define CONSOLE_BG_CYAN			"\033[0;46m"
#define CONSOLE_BG_RED			"\033[0;41m"
#define CONSOLE_BG_PURPLE		"\033[0;45m"
#define CONSOLE_BG_YELLOW		"\033[0;43m"
#define CONSOLE_BG_LIGHT_GREY	"\033[0;47m"
#define CONSOLE_BG_DARK_GREY	"\033[1;40m"
#define CONSOLE_BG_LIGHT_BLUE	"\033[1;44m"
#define CONSOLE_BG_LIGHT_GREEN	"\033[1;42m"
#define CONSOLE_BG_LIGHT_CYAN	"\033[1;46m"
#define CONSOLE_BG_LIGHT_RED	"\033[1;41m"
#define CONSOLE_BG_LIGHT_PURPLE	"\033[1;45m"
#define CONSOLE_BG_LIGHT_YELLOW	"\033[1;43m"
#define CONSOLE_BG_WHITE		"\033[1;47m"


void console_clean();
int console_getRow();
int console_getColumn();
int console_getWidth();
int console_getHeight();

/*
 * gets hit keys from the keyboard
 * bytes - how many bytes before action (0)
 * second - how many seconds before action (8)
 */
int console_getKey(int byte, int second);

#ifdef __cplusplus
}
#endif

#endif
