#ifndef LA_NETWORK_H
#define LA_NETWORK_H

#define NETWORK_ERROR_INIT 1
#define NETWORK_ERROR_OPTION 2
#define NETWORK_ERROR_INFORMATION 3
#define NETWORK_ERROR_CONNECTION 4
#define NETWORK_ERROR_BIND 5
#define NETWORK_ERROR_LISTEN 6
#define NETWORK_ERROR_ACCEPT 7
#define NETWORK_ERROR_READ 8
#define NETWORK_ERROR_WRITE 9

#define NETWORK_BUFFER_SIZE 1024

#include "la_exception.h"

typedef struct la_network NETWORK;
typedef struct la_network_client NETWORK_CLIENT;
#ifdef __WIN32
	#define NETWORK_SOCKET_ERROR SOCKET_ERROR
	typedef SOCKET NETWORK_SOCKET;
	typedef int NETWORK_SEND;
	typedef int NETWORK_RECV;
#else
	#define NETWORK_SOCKET_ERROR -1
	typedef int NETWORK_SOCKET;
	typedef ssize_t NETWORK_SEND;
	typedef ssize_t NETWORK_RECV;
#endif
typedef int NETWORK_PORT;

typedef void (*NETWORK_ACCEPT_CALLBACK)(NETWORK *self, void *object);

NETWORK *network_new();
void network_free(NETWORK *self);
void network_setException(NETWORK *self, EXCEPTION *e);
void network_setAddress(NETWORK *self, const char *adr);
void network_setPort(NETWORK *self, int port);
void network_setTimeout(NETWORK *self, int timeout);
void network_setQueue(NETWORK *self, int queue);
void network_clientOpen(NETWORK *self);
BOOL network_isOpen(NETWORK *self);
void network_close(NETWORK *self);
void network_serverOpen(NETWORK *self);
void network_serverAccept(NETWORK *self, NETWORK_ACCEPT_CALLBACK callback, void *object);
void network_writeData(NETWORK *self);
void network_readData(NETWORK *self);
void network_writeString(NETWORK *self, const char *str);
char *network_readString(NETWORK *self);
void network_writeNumber(NETWORK *self, int num);
int network_readNumber(NETWORK *self);
void network_writeFile(NETWORK *self, const char *filename);
int network_readFile(NETWORK *self, const char *filename);

NETWORK_SOCKET network_client_getSocket(NETWORK *self);
char *network_client_getAddress(NETWORK *self);
NETWORK_PORT network_client_getPort(NETWORK *self);

#endif
