#include <stdio.h>
#include <stdlib.h>
#include <la_exception.h>
#include <la_file.h>
#include <la_network.h>

void client(NETWORK *self, void *object) {
	printf("CLIENT:\n" );
	printf("\tSOCKET:\t\t%d\n", network_getAcceptSocket(self));
	char *adr = network_getAcceptAddress(self);
	printf("\tADDRESS:\t%s\n", adr);
	free(adr);
	printf("\tPORT:\t\t%d\n", network_getAcceptPort(self));

	char *s = network_readString(self);
	printf("\tSTRING:\t\t%s\n", s);
	free(s);

	BOOL b = network_readStatus(self);
	printf("\tSTATUS:\t\t%s\n", b ? "TRUE" : "FALSE");

	int n = network_readNumber(self);
	printf("\tNUMBER:\t\t%d\n", n);

	network_readFile(self, "network.bin");
	printf ( "\tFILE:\t\t%ld bytes\n", file_size("network.bin") );

	network_readData(self);
	printf ( "\tDATA:\t\t");
	size_t i;
	char *data = network_getDataBinary(self);
	for(i = 0; i < network_getDataSize(self); ++i) {
		printf ( ":%d:", data[i] );
		fflush(stdout);
	}
	printf ( " (%ld bytes)\n", network_getDataSize(self) );
	free(data);
	network_freeData(self);

	network_cancel(self);
}

void throwError(EXCEPTION *e, void *obj) {
	exception_show(e);
	exit(1);
}

int main(void) {
	EXCEPTION *e = exception_new();
	exception_addCallback(e, throwError, NULL);

	NETWORK *net = network_new();
	network_setException(net, e);
	network_setPort(net, 8888);
	network_open(net);
	network_callAccept(net, client, NULL);
	network_close(net);
	network_free(net);

	exception_free(e);

	return 0;
}
