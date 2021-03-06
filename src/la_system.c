/*
 * =====================================================================================
 *
 *       Filename:  la_system.c
 *    Description:  system specific functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "la_system.h"

#ifdef SYSTEM_OS_TYPE_WINDOWS
#undef BOOL
#include <windows.h>
//#pragma comment(lib, "user32.lib")
#else
#include <unistd.h>
#endif

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

#ifdef SYSTEM_OS_TYPE_LINUX
SYSTEM_UPTIME system_getUptime() {
	SYSTEM_UPTIME ut;
    memset(&ut, 0, sizeof(ut));                 /* set all fields to '0' */

	char line[64 + 1];

	FILE *file;
	file = fopen ("/proc/uptime", "r");
	if (file == NULL) return ut;
	char *rc = fgets(line, 64, file);
	fclose(file);
	if (rc == NULL) return ut;

	char *idx = strchr(line, ' ');
	if (idx == NULL) return ut;
	idx[0] = '\0';                              /* cut string */

	double value = atof(line);
//	printf ( "UPTIME: %f\n", value );

	ut.day = (int)(value / 86400);
	ut.hour = ((int)value % 86400) / 3600;
	ut.minute = ((int)value % 3600) / 60;
	ut.second = ((int)value % 60);
	ut.millisecond = (int)(value * 1000) - ((int)value * 1000);

	return ut;
}
#endif

BOOL system_isCopyright() {
	char *env = getenv("COPYRIGHT");
	if (env == NULL || strlen(env) < 1) return TRUE;


	return boolean_isTrue(env);
}

BOOL system_isDebug() {
	char *env = getenv("DEBUG");
	if (env == NULL || strlen(env) < 1) return FALSE;

	char chr = env[0];
	chr = tolower(chr);

	return boolean_isTrue(env);
}

int system_getOSArch() {
    FILE *fp = NULL;
    char cb64[3];

    fp = popen ("getconf LONG_BIT", "r");
    if (!fp)
       return -1;

    char *p = fgets(cb64, 3, fp);

	pclose(fp);
    if (!p)
        return -2;

    if (!strncmp (cb64, "16", 3))
        return 16;
    else if (!strncmp (cb64, "32", 3))
        return 32;
    else if (!strncmp (cb64, "64", 3))
        return 64;
	else return -3;
}

int system_getCPUArch() {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo);
	switch (siSysInfo.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_AMD64:
		case PROCESSOR_ARCHITECTURE_IA64:
			return 64;
		case PROCESSOR_ARCHITECTURE_INTEL:
			return 32;
		default:
			return -1;
	}
#else
	FILE *fp = NULL;
	char line[2048];
	char *flags = NULL;

	fp = fopen("/proc/cpuinfo", "r");
	if (!fp)
		return -1;


	while (fgets(line, 2048, fp)) {
		flags = strstr(line, "flags");
		if (flags != NULL)
			break;
	}

	fclose(fp);

	if (strstr(flags, " lm ")) {
		return 64;
	} else if (strstr(flags, " tm ")) {
		return 32;
	} else if (strstr(flags, " rm ")) {
		return 16;
	} else return -1;
#endif
}

int system_getBinaryArch() {
#ifdef SYSTEM_OS_TYPE_LINUX
	return __WORDSIZE;
#else
	return -1;
#endif
}

int system_getCompilerArch() {
#if defined __LP64__ || defined __x86_64__
	return 64;
#elif defined __i386__
	return 32;
#endif
	return -1;
}

void system_sleep(unsigned long milliseconds) {
#ifdef __WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000);
#endif
}

#ifdef __cplusplus
namespace la {
	namespace system {
		bool isOSTypeWindows() {
			return system_isOSTypeWindows();
		}
		bool isOSTypeDOS() {
			return system_isOSTypeDOS();
		}
		bool isOSTypeOS2() {
			return system_isOSTypeOS2();
		}
		bool isOSTypeUNIX() {
			return system_isOSTypeUNIX();
		}
		bool isOSTypeLinux() {
			return system_isOSTypeLinux();
		}
		bool isOSTypeIRIX() {
			return system_isOSTypeIRIX();
		}
		bool isOSTypeHPUX() {
			return system_isOSTypeHPUX();
		}
		bool isOSTypeOSX();

		bool isOSVendorMicrosoft() {
			return system_isOSVendorMicrosoft();
		}
		bool isOSVendorSGI() {
			return system_isOSVendorSGI();
		}
		bool isOSVendorHP() {
			return system_isOSVendorHP();
		}
		bool isOSVendorIBM() {
			return system_isOSVendorIBM();
		}
		bool isOSVendorApple() {
			return system_isOSVendorApple();
		}

		bool isArch32() {
			return system_isArch32();
		}

		bool isArch64() {
			return system_isArch64();
		}

	#ifdef SYSTEM_OS_TYPE_LINUX
		int getOSArch() {
			return system_getOSArch();
		}

		int getCPUArch() {
			return system_getCPUArch();
		}

		int getBinaryArch() {
			return system_getBinaryArch();
		}

		int getCompilerArch() {
			return system_getCPUArch();
		}

		Uptime::Uptime() {
			obj = system_getUptime();
		}

		int Uptime::getDay() {
			return obj.day;
		}

		int Uptime::getHour() {
			return obj.hour;
		}

		int Uptime::getMinute() {
			return obj.minute;
		}

		int Uptime::getSecond() {
			return obj.second;
		}

		int Uptime::getMillisecond() {
			return obj.millisecond;
		}
	#endif

		bool isCopyright() {
			return system_isCopyright();
		}

		bool isDebug() {
			return system_isDebug();
		}
		void sleep(unsigned long milliseconds) {
			system_sleep(milliseconds);
		}
	}
}
#endif
