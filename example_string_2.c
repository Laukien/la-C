#include "la_list.h"
#include "la_string.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	LIST *tokens = string_split("a number of words to be tokenized", " ");

	list_show(tokens);
	list_free(tokens);

	return EXIT_SUCCESS;
}
