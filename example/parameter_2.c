#include "la_file.h"
#include "la_parameter.h"
#include "stdio.h"

int main(void) {

	PARAMETER *param = parameter_new();

	parameter_add(param, "key1", "valueA");
	parameter_add(param, "key2", "valueB");
	parameter_saveToFile(param, "test.param");

	parameter_reset(param);

	unsigned int count = parameter_loadFromFile(param, "test.param");
	printf ( "Load %d keys.\n", count );
	printf ( "KEY 2: %s\n", parameter_get(param, "key2") );

	parameter_free(param);
	file_remove("test.param");
	
	return(0);
}
