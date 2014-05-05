/*
 * =====================================================================================
 *
 *       Filename:  la_system.h
 *    Description:  system specific functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_SYSTEM_H
#define LA_SYSTEM_H

#include "la_boolean.h"

#ifdef __cplusplus
extern "C" {
#endif

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
BOOL system_isOSTypeWindows();
BOOL system_isOSTypeDOS();
BOOL system_isOSTypeOS2();
BOOL system_isOSTypeUNIX();
BOOL system_isOSTypeLinux();
BOOL system_isOSTypeIRIX();
BOOL system_isOSTypeHPUX();
BOOL system_isOSTypeOSX();

/* OS-Vendor */
BOOL system_isOSVendorMicrosoft();
BOOL system_isOSVendorSGI();
BOOL system_isOSVendorHP();
BOOL system_isOSVendorIBM();
BOOL system_isOSVendorApple();

/* Arch */
BOOL system_isArch32();
BOOL system_isArch64();
#ifdef SYSTEM_OS_TYPE_LINUX
int system_getOSArch();
#endif
int system_getCPUArch();
#ifdef SYSTEM_OS_TYPE_LINUX
int system_getBinaryArch();
int system_getCompilerArch();
#endif

/* OS */
#ifdef SYSTEM_OS_TYPE_LINUX
typedef struct {
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
} SYSTEM_UPTIME;

SYSTEM_UPTIME system_getUptime();
#endif

/* OTHER */
BOOL system_isCopyright();
BOOL system_isDebug();

void system_sleep(unsigned long milliseconds);

#ifdef __cplusplus
}

namespace la {
	namespace system {
		bool isOSTypeWindows();
		bool isOSTypeDOS();
		bool isOSTypeOS2();
		bool isOSTypeUNIX();
		bool isOSTypeLinux();
		bool isOSTypeIRIX();
		bool isOSTypeHPUX();
		bool isOSTypeOSX();

		bool isOSVendorMicrosoft();
		bool isOSVendorSGI();
		bool isOSVendorHP();
		bool isOSVendorIBM();
		bool isOSVendorApple();

#ifdef SYSTEM_OS_TYPE_LINUX
		bool isArch32();
		bool isArch64();
		int getOSArch();
		int getCPUArch();
		int getBinaryArch();
		int getCompilerArch();
		class Uptime {
			private:
				SYSTEM_UPTIME obj;
			public:
				Uptime();
				int getDay();
				int getHour();
				int getMinute();
				int getSecond();
				int getMillisecond();
		};
#endif

		bool isCopyright();
		bool isDebug();
		void sleep(unsigned long milliseconds);
	}
}
#endif

#endif
