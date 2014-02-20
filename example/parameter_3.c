#include "la_file.h"
#include "la_parameter.h"
#include "stdio.h"

int main(int argc, char *argv[]) {

	PARAMETER *param = parameter_new();
	parameter_addArgument(param, argc, argv);
	parameter_add(param, "key1", "valueA");

	parameter_show(param);

	parameter_free(param);
	
	return(0);
}
