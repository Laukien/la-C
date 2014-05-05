#include <ctime>
#include <iostream>
#include "la_system.h"
#include "la_thread.h"

using namespace std;
using namespace la::thread;

int thread(void *arg) {
	int *i = (int *)arg;

	la::system::sleep(rand() % 1000);
	cout << "THREAD: " << *i << endl;

	return *i;
}

int main(void) {
	srand(time(NULL));

	int i1 = 1;
	Thread t1;
	t1.setFunction(thread, &i1);
	t1.run();

	int i2 = 2;
	Thread t2;
	t2.setFunction(thread, &i2);
	t2.run();

	int i3 = 3;
	Thread t3;
	t3.setFunction(thread, &i3);
	t3.run();

	t1.wait();
	t2.wait();
	t3.wait();

	return EXIT_SUCCESS;
}
