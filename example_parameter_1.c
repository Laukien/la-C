#include "la_parameter.h"
#include "stdio.h"

int main(void) {

	LA_PARAMETER *param = parameter_new();

	parameter_add(param, "key1", "valueA");
	parameter_add(param, "key2", "valueB");

	printf ( "KEY 1: %s\n", parameter_get(param, "key1") );

	parameter_free(param);

	return(0);
}
