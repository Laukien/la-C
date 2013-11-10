#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <la_exception.h>
#include <la_network.h>

int main(int argc, char *argv[]) {
	EXCEPTION *e = exception_new();

	NETWORK *net = network_new();
	network_setException(net, e);
	network_setAddress(net, "127.0.0.1");
	network_setPort(net, 8888);
	network_open(net);

	network_writeString(net, "string");
	sleep(1);
	network_writeStatus(net, TRUE);
	sleep(1);
	network_writeNumber(net, 123);
	sleep(1);
	network_writeFile(net, argv[0]);
	sleep(1);
	network_data_init(net);
	network_data_setBinary(net, "Test!", 5);
	network_writeData(net);
	network_data_free(net);

	network_close(net);
	network_free(net);

	exception_free(e);

	return 0;
}
