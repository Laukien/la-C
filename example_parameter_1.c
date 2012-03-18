#include "la_parameter.h"
#include "stdio.h"

int main(void) {


	LA_PARAMETER *param = parameter_new();

	parameter_add(param, "key1", "valueA");
	parameter_add(param, "key2", "valueB");

	printf ( "KEY 2: %s\n", parameter_get(param, "key2") );

	parameter_free(param);

	return(0);
}
