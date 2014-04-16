#include <iostream>
#include <string>
#include "la_system.h"

using namespace std;
using namespace la::system;

int main(void) {
	Uptime ut;

	cout << ut.getDay() << " days, ";
	cout << ut.getMinute() << " minutes, ";
	cout << ut.getSecond() << " seconds, ";
	cout << ut.getMillisecond() << " milliseconds";
	cout << endl;

	return 0;
}
