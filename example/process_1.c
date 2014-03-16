/*
 * gcc -I . -I /usr/local/include -o process_1 process_1.c la_process.c /usr/local/lib/libla.a && ./process_1
 * i686-pc-mingw32-gcc -I . -I /usr/local/include -o process_1.exe process_1.c la_process.c ../../ext/la-C/lib/la.1.8.3.lib && wine process_1.exe
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef __WIN32
#include <windows.h>
#endif
#include <la_process.h>

int main() {
	PROCESS *p;
	p = process_new();
	process_setWait(p, FALSE);
	printf("IN:  stdin\n");
	process_setInputFromString(p, "stdin");

#ifdef __WIN32
	process_setWorkDirectory(p, "C:\\windows");
	process_execute(p, "notepad.exe win.ini");
#else
	process_setWorkDirectory(p, "/var");
	process_execute(p, "/bin/ls -l");
#endif

	/* wait */
	printf("WAIT: ");
	fflush(stdout);
	while (process_getStatus(p) == PROCESS_STATUS_RUNNING) {
#ifdef __WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		printf(".");
		fflush(stdout);
	}
	printf("\n");

	if (process_getStatus(p) != PROCESS_STATUS_FINISHED) {
		int stat = process_getStatus(p);
		printf ( "ERROR: %d => ", stat );
		switch (stat) {
			case PROCESS_STATUS_ERROR:
				printf ( "unable to start process\n" );
				break;
			case PROCESS_STATUS_TERMINATED:
				printf ( "process terminated\n" );
				break;
			default:
				printf ( "unknown\n" );
		}
		printf ( "\n" );
	} else {
		/* exit */
		printf("EXIT: %d\n", process_getExit(p));

		/* result */
		char *out = process_getOutputAsString(p);
		printf("OUT: %s\n", out);
		free(out);
		char *err = process_getErrorAsString(p);
		printf("ERR: %s\n", err);
		free(err);
	}

	process_free(p);

	return 0;
}
