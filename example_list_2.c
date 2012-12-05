#include "la_list.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {

	LIST *param = list_new();

	list_add(param, "valueA");
	list_add(param, "valueX");
	list_add(param, "valueB");

	list_remove(param, 1);

	list_show(param);

	list_free(param);

	return(0);
}

