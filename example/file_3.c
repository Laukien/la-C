#include "la_boolean.h"
#include "la_file.h"
#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	LIST *list;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	list = file_list("C:\\WINDOWS", TRUE);
#else
	list = file_list("/etc", TRUE);
#endif

	list_show(list);

	list_free(list);

	return 0;
}
