#include "la_number.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char *num_i;
	num_i = number_integerToString(-1234);
	printf ( "Number: %s (%d)\n", num_i, (int)number_getIntegerLength(-1234) );
	free(num_i);

	char *num_ui;
	num_ui = number_unsignedIntegerToString(1234);
	printf ( "Number: %s (%d)\n", num_ui, (unsigned int)number_getUnsignedIntegerLength(1234) );
	free(num_ui);

	return 0;
}
