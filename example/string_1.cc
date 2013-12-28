#include <iostream>
#include <string>
#include "la_string.h"

using namespace std;
using namespace la::string;

int main(void) {

	string text = "ThE ExamplE!";
	string all = replace(text, "E", "e");
	string first = replaceFirst(all, "The", "This is an");

	cout << "text: " << text << endl;
	cout << "all: " << all << endl;
	cout << "first: " << first << endl;

	return 0;
}
