#include <string>
#include <cstdlib>
#include "la_wrapper.h"
#include "la_datetime.h"
#include "la_file.h"
#include "la_directory.h"

namespace datetime {
	std::string getTimestampAsString() {
		char *ts = datetime_getTimestampAsString();
		std::string tss = ts;
		free(ts);

		return tss;
	}
}

namespace file {
	bool exists(const std::string filename) {
		return file_exists(filename.c_str());
	}

	std::string name(const std::string filename) {
		char *name = file_name(filename.c_str());	
		std::string names = name;
		free(name);

		return names;
	}

	std::string temp() {
		char *file = file_temp();
		std::string tmp = file;
		free(file);

		return tmp;
	}
}

namespace directory {
	bool exists(const std::string name) {
		return directory_exists(name.c_str());
	}

	std::string temp() {
		char *dir = directory_temp();
		std::string tmp = dir;
		free(dir);

		return tmp;
	}
}
