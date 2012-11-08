#include "la_list.h"
#include "stdio.h"

int main(void) {

	LIST *param = list_new();

	list_add(param, "valueA");

	printf ( "Index 1: %s\n", list_get(param, 0) );

	list_free(param);

	return(0);
}

