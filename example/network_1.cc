#include <iostream>
#include <la_network.h>

using namespace std;
using namespace la::network;

int main() {
	string host = "en.wikipedia.org";           /* wikipedia */
	string word;

	cout << "Searching Wikipedia: " << flush;
	cin >> word;
	word[0] = toupper(word[0]);                 /* upper first letter */

	Network net;
	net.setAddress(host);
	net.setPort(80);
	net.setTimeout(5);
	try {
		net.open();
		net.writeString("GET /wiki/" + word + " HTTP/1.1");
		net.writeString("Host: " + host);
		net.writeString("User-Agent: ELinks/0.12pre5 (textmode; Linux; 167x55-2)");
		net.writeString("Accept: */*");
		net.writeString("Connection: Keep-Alive");
		net.writeString("Accept-Language: en");
		net.writeString("");

		string page = net.readString();
        cout << page << endl;                   /* show result */

		net.close();
	} catch(la::exception::NetworkException &e) {
		cerr << "ERROR: " << e.getMessage() <<  endl;
	}
}
