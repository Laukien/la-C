#include <iostream>
#include <la_network.h>

using namespace std;
using namespace la::network;

int main() {
	string word;

	cout << "Enter one word-search: " << flush;
	cin >> word;

	Network net;
	net.setAddress("www.google.com");
	net.setPort(80);
	net.setTimeout(5);
	net.open();
	net.writeString("GET /search?q=" + word + " HTTP 1.1");
	string page = net.readString();
	cout << page << endl;
	net.close();
}
