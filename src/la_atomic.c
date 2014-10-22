/*
 * =====================================================================================
 *
 *       Filename:  la_atomic.c
 *    Description:  synchronize code
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include "la_atomic.h"

struct la_atomic {
#ifdef __WIN32
	HANDLE mutex;
#else
	pthread_mutex_t mutex;
#endif
};

ATOMIC *atomic_new() {
	ATOMIC *self = (ATOMIC *) malloc(sizeof(ATOMIC));
	if (!self) {
		fprintf(stderr,"\ndynamic memory allocation failed\n");
		exit (EXIT_FAILURE);
	}

#ifdef __WIN32
	self->mutex = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&(self->mutex), NULL);
#endif

	return self;
}

void atomic_free(ATOMIC *self) {
	assert(self);

	atomic_unlock(self);
#ifdef __WIN32
	CloseHandle(self->mutex);
#else
	pthread_mutex_destroy(&(self->mutex));
#endif
	free(self);
	self = NULL;
}

void atomic_lock(ATOMIC *self) {
#ifdef __WIN32
	WaitForSingleObject(self->mutex, INFINITE);
#else
	pthread_mutex_lock(&(self->mutex));
#endif
}

void atomic_unlock(ATOMIC *self) {
#ifdef __WIN32
	ReleaseMutex(self->mutex);
#else
	pthread_mutex_unlock(&(self->mutex));
#endif
}

#ifdef __cplusplus
namespace la {
	namespace atomic {
		Atomic::Atomic() {
			this->obj = atomic_new();
		}
		Atomic::~Atomic() {
			atomic_free(this->obj);
		}
		void Atomic::lock() {
			atomic_lock(this->obj);
		}
		void Atomic::unlock() {
			atomic_unlock(this->obj);
		}
	}
}
#endif
