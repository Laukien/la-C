#include "la_parameter.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {

	PARAMETER *param = parameter_new();

	parameter_add(param, "key1", "valueA");
	parameter_add(param, "keyX", "valueX");
	parameter_add(param, "key2", "valueB");

	parameter_remove(param, "keyX");

	char *str;

	str = parameter_get(param, "key1");
	printf("KEY 1: %s\n", str);
	free(str);

	str = parameter_get(param, "keyX");
	printf("KEY X: %s\n", str);
	free(str);

	str = parameter_getValueByIndex(param, 1);
	printf("KEY 2: %s\n", str);
	free(str);

	parameter_free(param);

	return(0);
}
