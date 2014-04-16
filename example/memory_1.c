#include "la_memory.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	char buf[10];
	memory_set(buf, '.', 10);
	memory_print(buf, 10);
	putc('\n', stdout);
	memory_zero(buf, 10);
	memory_print(buf, 10);

	return 0;
}
