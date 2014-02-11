#include <iostream>
#include "la_parameter.h"

using namespace std;
using namespace la::parameter;

int main() {
	Parameter param;
	param.add("key1", "valueA");
	param.add("keyX", "valueX");
	param.add("key2", "valueB");

	cout << "BEFORE" << endl;
	param.show();

	param.remove("keyX");

	cout << "AFTER" << endl;
	param.show();

	return 0;
}
