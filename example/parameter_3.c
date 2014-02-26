#include "la_message.h"
#include "la_parameter.h"

int main(int argc, char *argv[]) {
	PARAMETER *param = parameter_new();

	if (!parameter_loadFromArguments(param, argc, argv)) {
		message_error("unable to load parameter-file");
		return 1;
	}
	parameter_add(param, "key1", "valueA");
	parameter_addReplace(param, "key1", "value1");

	parameter_show(param);

	parameter_free(param);
	
	return 0;
}
