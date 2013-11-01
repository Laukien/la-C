#include "la_error.h"

int main() {
	ERROR *err = error_new();

	error_set(err, 123, "test-error");
	error_setMessage(err, "Terrible internal error no. %d", 456);
	error_set(err, 789, "test-error");
	error_setComment(err, "Just a test!");
	error_setCause(err, "for learning");
	error_setAction(err, "nothing");

	error_show(err);

	error_free(err);

	return 0;
}
