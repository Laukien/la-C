#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef __WIN32
	#undef BOOL
	#include <windows.h>
	#include <winsock2.h>
	#include <io.h>
	typedef int SEND;
	typedef int RECV;
	typedef int socklen_t;
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#define SOCKET_ERROR -1
	typedef int SOCKET;
	typedef ssize_t SEND;
	typedef ssize_t RECV;
#endif
#include "la_boolean.h"
#include "la_message.h"
#include "la_number.h"
#include "la_network.h"

struct la_network {
	EXCEPTION *exception;
	SOCKET socket;
	BOOL connect;
	char *address;
	int port;
	int timeout;
	char *data;
	size_t size;
};

void throwError(NETWORK *self, int id, const char *message, const char *cause, const char *action) {
	assert(self);

	if (self->exception) {
		exception_setLong(self->exception, id, message, cause, action);
		exception_throw(self->exception);
	} else {
		char *num = number_integerToString(id);
		char *str = (char *) malloc(strlen(message) + 2 + strlen(num) + 1 + 1);
		strcpy(str, message);
		strcat(str, " (");
		strcat(str, num);
		strcat(str, ")");
		free(num);
		message_error(str);
	}
}

NETWORK *network_new() {
	NETWORK *self = (NETWORK *)malloc(sizeof(NETWORK));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, 0, sizeof(NETWORK));
	self->timeout = 10;

	return self;
}

void network_free(NETWORK *self) {
	assert(self);

	network_close(self);

	if (self->address) {
		free(self->address);
		self->address = NULL;
	}

	self->port = 0;

	if (self->data) {
		free(self->data);
		self->data = NULL;
	}

	self->size = 0;

	free(self);
	self = NULL;
}

void network_setException(NETWORK *self, EXCEPTION *e) {
	assert(self);
	assert(e);

	self->exception = e;
}

void network_setAddress(NETWORK *self, const char *adr) {
	assert(self);
	assert(adr);

	if (self->address) {
		free(self->address);
	}

	self->address = strdup(adr);
}

void network_setPort(NETWORK *self, int port) {
	assert(self);
	assert(port > 0);

	self->port = port;
}

void network_setTimeout(NETWORK *self, int timeout) {
	assert(self);
	assert(timeout > 0);

	self->timeout = timeout;
}

void network_openClient(NETWORK *self) {
	assert(self);
	assert(self->address);

#ifdef SYSTEM_OS_TYPE_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2 ,0), &wsaData)) {
		int errCode = WSAGetLastError();
		LPSTR errString = NULL;
		int size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errCode, 0, (LPSTR)&errString, 0, 0);
		throwError(self, NETWORK_ERROR_INIT, "unable to init winsock", errString, "check up Windows network stack");
		LocalFree(errString);
		return;
	}
#endif

	/* create socket */
	self->socket = socket(AF_INET, SOCK_STREAM, 0);

	/* timeout */
	struct timeval timeout;
	timeout.tv_sec = self->timeout;
	timeout.tv_usec = 0;
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		throwError(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}
	if (setsockopt (self->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		throwError(self, NETWORK_ERROR_OPTION, "unable to set send-timeout", strerror(errno), NULL);
		return;
	}

	/* connect to server */
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));         /* clean structure */

	/* check right address */
	unsigned long addr;
	if ((addr = inet_addr(self->address)) != INADDR_NONE) {
		memcpy((char *)&server.sin_addr, &addr, sizeof(addr));
	} else {
		struct hostent *host_info;
		host_info = gethostbyname(self->address);
		if (!host_info) {
			throwError(self, NETWORK_ERROR_INFORMATION, "unable to resolve address", strerror(errno), NULL);
			return;
		}
		memcpy((char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(self->port);

	int c = connect(self->socket, (struct sockaddr *)&server, sizeof(server));
	if (c == SOCKET_ERROR) {
		throwError(self, NETWORK_ERROR_CONNECTION, "unable to connect to server", strerror(errno), NULL);
		return;
	}
}
