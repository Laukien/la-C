#include <stdlib.h>
#include <stdio.h>
#include "la_string.h"

int main(void) {

	char *text = "ThE ExamplE!";
	char *all = string_replace(text, "E", "e");
	char *first = string_replaceFirst(all, "The", "This is an");

	printf ( "text: %s\n", text );
	printf ( "all: %s\n", all );
	printf ( "first: %s\n", first );

	free(first);
	free(all);

	exit (EXIT_SUCCESS);
}
