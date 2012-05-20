/*
 * =====================================================================================
 *
 *       Filename:  la_system.c
 *
 *    Description:  system-check
 *
 *        Version:  1.0
 *        Created:  03/09/2012 10:01:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BOOL system_isOSTypeWindows() {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeDOS() {
#ifdef SYSTEM_OS_TYPE_DOS
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeUNIX() {
#ifdef SYSTEM_OS_TYPE_UNIX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeLinux() {
#ifdef SYSTEM_OS_TYPE_LINUX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeIRIX() {
#ifdef SYSTEM_OS_TYPE_IRIX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeHPUX() {
#ifdef SYSTEM_OS_TYPE_HPUX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeOSX() {
#ifdef SYSTEM_OS_TYPE_OSX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSTypeOS2() {
#ifdef SYSTEM_OS_TYPE_OS2
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSVendorMicrosoft() {
#ifdef SYSTEM_OS_VENDOR_MICROSOFT
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSVendorSGI() {
#ifdef SYSTEM_OS_VENDOR_SGI
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSVendorHP() {
#ifdef SYSTEM_OS_VENDOR_HP
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSVendorIBM() {
#ifdef SYSTEM_OS_VENDOR_IBM
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isOSVendorApple() {
#ifdef SYSTEM_OS_VENDOR_APPLE
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isArch32() {
#ifdef SYSTEM_ARCH_32
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL system_isArch64() {
#ifdef SYSTEM_ARCH_64
	return TRUE;
#else
	return FALSE;
#endif
}

SYSTEM_UPTIME system_getUptime() {
#ifdef SYSTEM_OS_TYPE_LINUX
	SYSTEM_UPTIME ut;
    memset(&ut, 0, sizeof(ut));                 /* set all fields to '0' */

	char line[64];

	FILE *file;
	file = fopen ("/proc/uptime", "r");
	if (file == NULL) return ut;
	fgets(line, (sizeof(char) * sizeof(line)) - 1, file);
	fclose(file);

	char *idx = strchr(line, ' ');
	if (idx == NULL) return ut;
	idx[0] = '\0';                              /* cut string */

	double value = atof(line);
	printf ( "UPTIME: %f\n", value );

	ut.day = value / 86400;
	ut.hour = ((int)value % 86400) / 3600;
	ut.minute = ((int)value % 3600) / 60;
	ut.second = ((int)value % 60);
	ut.millisecond = (value * 1000) - ((int)value * 1000);

	return ut;
#else
#error not implemented yet
#endif
}
