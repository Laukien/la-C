#include <stdio.h>
#include <stdlib.h>
#include <la_exception.h>
#include <la_file.h>
#include <la_network.h>

void client(NETWORK *self, void *object) {
	printf("CLIENT:\n" );
	printf("\tSOCKET:\t\t%d\n", network_accept_getSocket(self));
	char *adr = network_accept_getAddress(self);
	printf("\tADDRESS:\t%s\n", adr);
	free(adr);
	printf("\tPORT:\t\t%d\n", network_accept_getPort(self));

	char *s = network_readString(self);
	printf("\tSTRING:\t\t%s\n", s);
	free(s);

	int n = network_readNumber(self);
	printf("\tNUMBER:\t\t%d\n", n);

	network_readFile(self, "network.bin");
	printf ( "\tFILE:\t\t%ld bytes\n", file_size("network.bin") );

	network_readData(self);
	printf ( "\tDATA:\t\t");
	size_t i;
	char *data = network_data_getContent(self);
	for(i = 0; i < network_data_getSize(self); ++i) {
		printf ( ":%d:", data[i] );
		fflush(stdout);
	}
	printf ( " (%ld bytes)\n", network_data_getSize(self) );
	free(data);
	network_data_free(self);
}

int main(void) {
	EXCEPTION *e = exception_new();

	NETWORK *net = network_new();
	network_setException(net, e);
	network_setPort(net, 8888);
	network_open(net);
	network_serverAccept(net, client, NULL);
	network_close(net);
	network_free(net);

	exception_free(e);

	return 0;
}
