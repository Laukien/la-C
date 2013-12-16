#include "la_file.h"
#include "la_system.h"

using namespace la::file;

int main(void) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	list("C:\\WINDOWS").show();
#else
	list("/etc").show();
#endif

	return 0;
}
