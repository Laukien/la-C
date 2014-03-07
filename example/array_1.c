#include <stdlib.h>
#include <stdio.h>
#include <la_array.h>

int main() {
	char *arg;
	ARRAY *v;
	v = array_new();

	/* create array from string */
	array_loadFromString(v, "arg1 arg3", ' ');

	/* show size of array */
	printf ( "SIZE: %ld\n", array_getSize(v) );

	/* show second argument */
	arg = array_getString(v, 1);
	printf("ARG 2: %s\n", arg);
	free(arg);

	/* change second argument */
	array_setString(v, 1, "arg2");

	/* show array */
	array_show(v);

	/* free resources */
	array_free(v);

	return 0;
}
