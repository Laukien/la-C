#include "la_directory.h"
#include "la_system.h"

using namespace la::directory;

int main(void) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	list("C:\\WINDOWS").show();
#else
	list("/etc").show();
#endif

	return 0;
}
