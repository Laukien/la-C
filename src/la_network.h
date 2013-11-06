#ifndef LA_NETWORK_H
#define LA_NETWORK_H

#define NETWORK_ERROR_INIT 1
#define NETWORK_ERROR_OPTION 2
#define NETWORK_ERROR_INFORMATION 3
#define NETWORK_ERROR_CONNECTION 4

#include "la_exception.h"

typedef struct la_network NETWORK;

NETWORK *network_new();
void network_free(NETWORK *self);
void network_setException(NETWORK *self, EXCEPTION *e);
void network_setAddress(NETWORK *self, const char *adr);
void network_setPort(NETWORK *self, int port);
void network_setTimeout(NETWORK *self, int timeout);
void network_openClient(NETWORK *self);
void network_openServer(NETWORK *self);
void network_close(NETWORK *self);
void network_sendData(NETWORK *self);
void network_recvData(NETWORK *self);
void network_sendString(NETWORK *self, const char *str);
char *network_recvString(NETWORK *self);
void network_sendNumber(NETWORK *self, int num);
int network_recvNumber(NETWORK *self);

#endif
