#include <string>

namespace datetime {
	std::string getTimestampAsString();
}

namespace file {
	bool exists(const std::string filename);
	std::string name(const std::string filename);
	std::string temp();
}

namespace directory {
	bool exists(const std::string name);
	std::string temp();
}

