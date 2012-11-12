#include "la_directory.h"
#include "la_system.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	LIST *list;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	list = directory_list("C:\\WINDOWS");
#else
	list = directory_list("/etc");
#endif

	list_show(list);

	return 0;
}
