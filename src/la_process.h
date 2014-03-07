/*
 * =====================================================================================
 *
 *       Filename:  la_process.h
 *    Description:  handle operating system processes
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_PROCESS_H
#define LA_PROCESS_H

#ifdef __WIN32
//#undef BOOL;
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <la_boolean.h>
#include <la_system.h>

typedef struct la_process PROCESS;

typedef enum la_process_status {
	PROCESS_STATUS_ERROR = -1,
	PROCESS_STATUS_NONE = 0,
	PROCESS_STATUS_RUNNING = 1,
	PROCESS_STATUS_FINISHED = 2,
	PROCESS_STATUS_TERMINATED = 3
} PROCESS_STATUS;

#ifdef SYSTEM_OS_TYPE_WINDOWS
typedef DWORD PROCESS_ID;
typedef DWORD PROCESS_EXIT;
#else
typedef pid_t PROCESS_ID;
typedef int PROCESS_EXIT;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	PROCESS *process_new();
	void process_free(PROCESS *self);
	void process_setWait(PROCESS *self, BOOL wait);
	BOOL process_isWait(PROCESS *self);
	void process_setDirectory(PROCESS *self, const char *dir);
	void process_setInputFromString(PROCESS *self, const char *str);
	void process_setInputFromFile(PROCESS *self, const char *file);
	void process_execute(PROCESS *self, const char *command);
	PROCESS_ID process_getId(PROCESS *self);
	PROCESS_EXIT process_getExit(PROCESS *self);
	PROCESS_STATUS process_getStatus(PROCESS *self);
	char *process_getInputAsString(PROCESS *self);
	char *process_getInputAsFilename(PROCESS *self);
	char *process_getOutputAsString(PROCESS *self);
	char *process_getOutputAsFilename(PROCESS *self);
	char *process_getErrorAsString(PROCESS *self);
	char *process_getErrorAsFilename(PROCESS *self);
	
#ifdef __cplusplus
}
#include <string>

namespace la {
	namespace process {
		class process {
			private:
				PROCESS *obj;
			public:
		};
	}
}
#endif

#endif
