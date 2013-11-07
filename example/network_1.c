#include <stdio.h>
#include <stdlib.h>
#include <la_exception.h>
#include <la_network.h>

void client(NETWORK *self, void *object) {
	printf("CLIENT:\n" );
	printf("\tSOCKET:\t\t%d\n", network_client_getSocket(self));
	char *adr = network_client_getAddress(self);
	printf("\tADDRESS:\t%s\n", adr);
	free(adr);
	printf("\tPORT:\t\t%d\n", network_client_getPort(self));

	char *s = network_readString(self);
	printf("\tSTRING:\t\t%s\n", s);
	free(s);

	int i= network_readNumber(self);
	printf("\tNUMBER:\t\t%d\n", i);
}

int main(void) {
	EXCEPTION *e = exception_new();

	NETWORK *net = network_new();
	network_setException(net, e);
	network_setPort(net, 8888);
	network_serverOpen(net);
	network_serverAccept(net, client, NULL);
	network_close(net);
	network_free(net);

	exception_free(e);

	return 0;
}
