#include "la_list.h"
#include "stdio.h"
#include "stdlib.h"

int main(void) {

	LIST *param = list_new();

	list_add(param, "value2");
	list_add(param, "value1");
	list_add(param, "valueB");
	list_add(param, "valueA");

	list_swap(param, 0, 3);
	list_swap(param, 1, 2);

	list_show(param);

	list_free(param);

	return(0);
}


