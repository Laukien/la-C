#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "la_atomic.h"
#include "la_system.h"
#include "la_thread.h"

static ATOMIC *a;

int thread(void *arg) {
	int *i = (int *)arg;

	atomic_lock(a);

	system_sleep(rand() % 1000);
	printf ( "THREAD: %d\n", *i );

	atomic_unlock(a);

	return *i;
}

int main(void) {
	srand(time(NULL));

	a = atomic_new();

	system_sleep(10);
	int i1 = 1;
	THREAD *t1 = thread_new();
	thread_setFunction(t1, thread, &i1);
	thread_run(t1);

	system_sleep(10);
	int i2 = 2;
	THREAD *t2 = thread_new();
	thread_setFunction(t2, thread, &i2);
	thread_run(t2);

	system_sleep(10);
	int i3 = 3;
	THREAD *t3 = thread_new();
	thread_setFunction(t3, thread, &i3);
	thread_run(t3);

	thread_wait(t1);
	thread_free(t1);

	thread_wait(t2);
	thread_free(t2);

	thread_wait(t3);
	thread_free(t3);

	atomic_free(a);

	return EXIT_SUCCESS;
}
