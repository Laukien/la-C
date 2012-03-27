#include "la_number.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *num_i;
	num_i = number_intToString(-1234);
	printf ( "Number: %s (%d)\n", num_i, number_intToSize(-1234) );
	free(num_i);

	char *num_ui;
	num_ui = number_uintToString(1234);
	printf ( "Number: %s (%d)\n", num_ui, number_uintToSize(1234) );
	free(num_ui);

	return 0;
}
