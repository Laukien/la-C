#include "la_list.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {

	LIST *param = list_new();

	list_add(param, "valueA");

	char *str = list_get(param, 0);
	printf ( "Index 1: %s\n", str );
	free(str);

	list_free(param);

	return(0);
}

