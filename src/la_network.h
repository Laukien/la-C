#ifndef LA_NETWORK_H
#define LA_NETWORK_H

#define NETWORK_ERROR_INIT 1
#define NETWORK_ERROR_SYSTEM 2
#define NETWORK_ERROR_OPTION 3
#define NETWORK_ERROR_INFORMATION 4
#define NETWORK_ERROR_CONNECTION 5
#define NETWORK_ERROR_BIND 6
#define NETWORK_ERROR_LISTEN 7
#define NETWORK_ERROR_ACCEPT 8
#define NETWORK_ERROR_READ 9
#define NETWORK_ERROR_WRITE 10

#define NETWORK_BUFFER_SIZE 1024                /* 1 KB */
#define NETWORK_DATA_SIZE 1024 * 1024           /* 1 MB */

#include "la_system.h"
#include "la_exception.h"

typedef struct la_network NETWORK;
typedef struct la_network_accept NETWORK_ACCEPT;
typedef struct la_network_data NETWORK_DATA;

#ifdef SYSTEM_OS_TYPE_WINDOWS
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
void network_open(NETWORK *self);
BOOL network_isOpen(NETWORK *self);
void network_close(NETWORK *self);
void network_serverAccept(NETWORK *self, NETWORK_ACCEPT_CALLBACK callback, void *object);
void network_writeString(NETWORK *self, const char *str);
char *network_readString(NETWORK *self);
void network_writeNumber(NETWORK *self, int num);
int network_readNumber(NETWORK *self);
void network_writeFile(NETWORK *self, const char *filename);
void network_readFile(NETWORK *self, const char *filename);
void network_writeData(NETWORK *self);
void network_readData(NETWORK *self);

NETWORK_SOCKET network_accept_getSocket(NETWORK *self);
char *network_accept_getAddress(NETWORK *self);
NETWORK_PORT network_accept_getPort(NETWORK *self);
BOOL network_accept_exists(NETWORK *self);


void network_data_init(NETWORK *self);
void network_data_free(NETWORK *self);
void network_data_setLimit(NETWORK *self, size_t limit);
size_t network_data_getLimit(NETWORK *self);
size_t network_data_getSize(NETWORK *self);
void network_data_setBinary(NETWORK *self, const char *content, size_t size);
void network_data_setString(NETWORK *self, const char *content);
char *network_data_getBinary(NETWORK *self);
char *network_data_getString(NETWORK *self);

#endif
