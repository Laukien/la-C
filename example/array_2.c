/*
 * gcc -I . -I /usr/local/include -o array_1 array_1.c la_array.c /usr/local/lib/libla.a
 */
#include <stdlib.h>
#include <stdio.h>
#include <la_array.h>

int main() {
	char **args;
	ARRAY *v;
	v = array_new();

	/* create array from string */
	array_loadFromString(v, "arg1 arg2", ' ');

	/* show size of array */
	printf ( "SIZE: %ld\n", array_getSize(v) );

	/* show second argument */
	args = array_getArray(v);

	/* show array */
	array_showArray(args);

	/* free resources */
	array_freeArray(args);
	array_free(v);

	return 0;
}
