/*
 * =====================================================================================
 *
 *       Filename:  la_system.h
 *
 *    Description:  header with definitions the help to detect the system
 *
 *        Version:  1.0
 *        Created:  03/09/2012 09:36:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef LA_SYSTEM_H
#define LA_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "la_common.h"

/* OS-Type & OS-Vendor*/
#if defined WIN32 || defined _WIN32 || defined _WIN32_ || defined __WIN32 || defined __WIN32__ || defined _MSC_VER || defined __NT__ || defined _Windows
	#define SYSTEM_OS_VENDOR_MICROSOFT
	#define SYSTEM_OS_TYPE_WINDOWS
#elif defined MSDOS || defined _MSDOS || defined _MSDOS_ || defined __MSDOS || defined __MSDOS__ || defined __BORLANDC__ || defined __TURBOC__
	#define SYSTEM_OS_VENDOR_MICROSOFT
	#define SYSTEM_OS_TYPE_DOS
#elif defined __UNIX || defined __UNIX__
	#define SYSTEM_OS_TYPE_UNIX
#elif defined __linux || defined __linux__
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_LINUX
#elif defined _SGI_SOURCE || defined _MIPS_ISA
	#define SYSTEM_OS_VENDOR_SGI
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_IRIX
#elif defined__hpux 
	#define SYSTEM_OS_VENDOR_HP
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_HPUX
#elif defined __APPLE__ || defined __MACH__
	#define SYSTEM_OS_VENDOR_APPLE
	#define SYSTEM_OS_TYPE_UNIX
	#define SYSTEM_OS_TYPE_OSX
#elif defined __OS2__ 
	#define SYSTEM_OS_VENDOR_IBM
	#define SYSTEM_OS_TYPE_OS2
#else
	#define SYSTEM_OS_TYPE_UNKNOWN
#endif

#if defined __LP64__ || defined __x86_64__
	#define SYSTEM_ARCH_64
#elif defined __i386__
	#define SYSTEM_ARCH_32
#endif

/* OS-Type */
BOOL isOSTypeWindows();
BOOL isOSTypeDOS();
BOOL isOSTypeOS2();
BOOL isOSTypeUNIX();
BOOL isOSTypeLinux();
BOOL isOSTypeIRIX();
BOOL isOSTypeHPUX();
BOOL isOSTypeOSX();

/* OS-Vendor */
BOOL isOSVendorMicrosoft();
BOOL isOSVendorSGI();
BOOL isOSVendorHP();
BOOL isOSVendorIBM();
BOOL isOSVendorApple();

/* ARCH */
BOOL isArch32();
BOOL isArch64();

#ifdef __cplusplus
}
#endif

#endif
