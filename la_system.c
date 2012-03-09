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

BOOL isOSTypeWindows() {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeDOS() {
#ifdef SYSTEM_OS_TYPE_DOS
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeUNIX() {
#ifdef SYSTEM_OS_TYPE_UNIX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeLinux() {
#ifdef SYSTEM_OS_TYPE_LINUX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeIRIX() {
#ifdef SYSTEM_OS_TYPE_IRIX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeHPUX() {
#ifdef SYSTEM_OS_TYPE_HPUX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeOSX() {
#ifdef SYSTEM_OS_TYPE_OSX
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSTypeOS2() {
#ifdef SYSTEM_OS_TYPE_OS2
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSVendorMicrosoft() {
#ifdef SYSTEM_OS_VENDOR_MICROSOFT
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSVendorSGI() {
#ifdef SYSTEM_OS_VENDOR_SGI
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSVendorHP() {
#ifdef SYSTEM_OS_VENDOR_HP
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSVendorIBM() {
#ifdef SYSTEM_OS_VENDOR_IBM
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL isOSVendorApple() {
#ifdef SYSTEM_OS_VENDOR_APPLE
	return TRUE;
#else
	return FALSE;
#endif
}
