/*
 * =====================================================================================
 *
 *       Filename:  la_tread.h
 *    Description:  common thread-handling
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_THREAD_H
#define LA_THREAD_H

#include "la_boolean.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	THREAD_STATUS_ERROR = -1,
	THREAD_STATUS_NONE = 0,
	THREAD_STATUS_RUNNING = 1,
	THREAD_STATUS_CANCELED = 2,
	THREAD_STATUS_KILLED = 3,
	THREAD_STATUS_EXIT = 4
} la_thread_status_t;

typedef struct la_thread THREAD;
typedef int (*THREAD_CALLBACK)(void *object);

THREAD *thread_new();
void thread_free(THREAD *self);
void thread_setFunction(THREAD *self, THREAD_CALLBACK function, void *argument);
void thread_run(THREAD *self);
void thread_cancel(THREAD *self);
void thread_kill(THREAD *self);
void thread_wait(THREAD *self);
int thread_getStatus(THREAD *self);
BOOL thread_isRunning(THREAD *self);
int thread_getExit(THREAD *self);

#ifdef __cplusplus
}

namespace la {
	namespace thread {
		class Thread {
			private:
				THREAD *obj;
			public:
				Thread();
				~Thread();
				void setFunction(THREAD_CALLBACK function, void *argument);
				void run();
				void cancel();
				void kill();
				void wait();
				int getStatus();
				bool isRunning();
				int getExit();
		};
	}
}
#endif

#endif
