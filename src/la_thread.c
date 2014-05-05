/*
 * =====================================================================================
 *
 *       Filename:  la_tread.c
 *    Description:  common thread-handling
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#include "la_thread.h"

struct la_thread {
#ifdef __WIN32
	HANDLE thread;
	volatile DWORD id;
#else
	pthread_t *thread;
	volatile long id;
#endif
	THREAD_CALLBACK function;
	void *argument;
	int exit;
	volatile la_thread_status_t status;
};

THREAD *thread_new() {
	THREAD *self = (THREAD *)malloc(sizeof(THREAD));

	self->thread = NULL;
	self->function = NULL;
	self->argument = NULL;
	self->exit = -1;
	self->status = THREAD_STATUS_NONE;
	self->id = -1;

	return self;
}

void thread_free(THREAD *self) {
	assert(self);

	if (self->thread) {
#ifdef __WIN32
		if (self->status == THREAD_STATUS_RUNNING) {
			thread_cancel(self);
			WaitForSingleObject(self->thread, 1000);
			if (self->status == THREAD_STATUS_RUNNING) {
				thread_kill(self);
			}
		}

		WaitForSingleObject(self->thread, INFINITE);
		CloseHandle(self->thread);
#else
		if (self->status == THREAD_STATUS_RUNNING) {
			thread_cancel(self);
			int count;
			while (self->status == THREAD_STATUS_RUNNING && count++ < 100) {
				usleep(1000);
			}
			if (self->status == THREAD_STATUS_RUNNING) {
				thread_kill(self);
			}
		}

		pthread_join(*(self->thread), NULL);
#endif
	}
	
	free(self->thread);
	self->thread = NULL;
	free(self);
	self = NULL;
}

void thread_setFunction(THREAD *self, THREAD_CALLBACK function, void *argument) {
	assert(self);
	assert(!self->function);
	self->function = function;
	self->argument = argument;
}

void _thread_clean(void *arg) {
	assert(arg);
	THREAD *self = (THREAD *)arg;

	self->status = THREAD_STATUS_EXIT;
}

#ifdef __WIN32
DWORD WINAPI _thread_run(LPVOID arg) {
#else
void *_thread_run(void *arg) {
#endif
	assert(arg);
	THREAD *self = (THREAD *)arg;

#ifndef __WIN32
	self->id = pthread_self();
#endif

	THREAD_CALLBACK f = self->function;
	self->status = THREAD_STATUS_RUNNING;
	self->exit = -1;

	self->exit = f(self->argument);

	if (self->status == THREAD_STATUS_RUNNING) {
		self->status = THREAD_STATUS_EXIT;
	}

	return 0;
}

void thread_run(THREAD *self) {
	assert(self);
	assert(!self->thread);
	assert(self->status == THREAD_STATUS_NONE);

#ifdef __WIN32
	DWORD id;
	self->thread = CreateThread(NULL, 0, _thread_run, self, 0, &id);
	self->id = id;
	if (self->thread == NULL) {
		self->status = THREAD_STATUS_ERROR;
	} else {
		self->status = THREAD_STATUS_RUNNING;
	}
#else
	self->thread = (pthread_t *)malloc(sizeof(pthread_t));

	if (pthread_create(self->thread, NULL, _thread_run, self)) {
		self->status = THREAD_STATUS_ERROR;
	} else {
		self->status = THREAD_STATUS_RUNNING;
	}
#endif
}

void thread_cancel(THREAD *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		PostThreadMessage(self->id, WM_QUIT, (WPARAM) NULL, (LPARAM) NULL);
#else
		pthread_cancel(*(self->thread));
#endif
		self->status = THREAD_STATUS_CANCELED;
	}
}

void thread_kill(THREAD *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		TerminateThread(self->thread, 0);
#else
		int old_state;
		int old_type;
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_type);

		pthread_cancel(*(self->thread));

		pthread_setcanceltype(old_type, NULL);
		pthread_setcancelstate(old_state, NULL);
#endif
		self->status = THREAD_STATUS_KILLED;
	}
}

void thread_wait(THREAD *self) {
	assert(self);
	assert(self->thread);

#ifdef __WIN32
	WaitForSingleObject(self->thread, INFINITE);
#else
	pthread_join(*(self->thread), NULL);
#endif
}

int thread_getStatus(THREAD *self) {
	assert(self);
	assert(self->thread);

	return self->status;
}

int thread_isRunning(THREAD *self) {
	assert(self);
	assert(self->thread);

	return self->status == THREAD_STATUS_RUNNING;
}

int thread_getExit(THREAD *self) {
	assert(self);
	assert(self->thread);

	if (self->status == THREAD_STATUS_RUNNING) {
#ifdef __WIN32
		WaitForSingleObject(self->thread, INFINITE);
#else
		pthread_join(*(self->thread), NULL);
#endif
	}

	return self->exit;
}
