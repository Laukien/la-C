#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <la_string.h>

int main(int argc, char *argv[]) {
	char *filename;

	if (argc == 2) {
		filename = (char *) malloc(strlen(argv[1] + 1));
		strcpy(filename, argv[1]);
	} else {
		filename = (char *) malloc(strlen(argv[0] + 2 + 1));
		strcpy(filename, argv[0]);
		strcat(filename, ".c");
	}

	printf ( "Filename: %s\n", filename );
	char *str = string_loadFromFile(filename);

	if (str != NULL) {
		printf ( "String:\n%s\n", str );
		free(str);
	} else
		fprintf(stderr, "Error: Unable to read file.\n");

	free(filename);

	return (EXIT_SUCCESS);
}
