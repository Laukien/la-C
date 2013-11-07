#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "la_boolean.h"
#include "la_message.h"
#include "la_number.h"
#include "la_string.h"
#include "la_network.h"
#ifdef SYSTEM_OS_TYPE_WINDOWS
	#undef BOOL
	#include <windows.h>
	#include <winsock2.h>
	#include <io.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
#endif

struct la_network {
	EXCEPTION *exception;
	NETWORK_SOCKET socket;
	NETWORK_CLIENT *client;
	BOOL connect;
	char *address;
	int port;
	int timeout;
	int queue;
	char *data;
	size_t size;
};

struct la_network_client {
	int socket;
	char *address;
	int port;
};

/* define internal client functions */
void network_client_init(NETWORK *self);
void network_client_setSocket(NETWORK *self, NETWORK_SOCKET socket);
void network_client_setAddress(NETWORK *self, const char *address);
void network_client_setPort(NETWORK *self, NETWORK_PORT port);
void network_client_close(NETWORK *self);
void network_client_free(NETWORK *self);

void _network_error(NETWORK *self, int id, const char *message, const char *cause, const char *action) {
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

void _network_open(NETWORK *self) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2 ,0), &wsaData)) {
		int errCode = WSAGetLastError();
		LPSTR errString = NULL;
		int size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errCode, 0, (LPSTR)&errString, 0, 0);
		_network_error(self, NETWORK_ERROR_INIT, "unable to init winsock", errString, "check up Windows network stack");
		LocalFree(errString);
		return;
	}
#endif
}

NETWORK *network_new() {
	NETWORK *self = (NETWORK *)malloc(sizeof(NETWORK));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, 0, sizeof(NETWORK));
	self->socket = 0;
	self->client = NULL;
	self->timeout = 10;
	self->queue = 3;

	return self;
}

void network_free(NETWORK *self) {
	assert(self);

	if (network_isOpen(self)) {
		network_close(self);
	}

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

void network_clientOpen(NETWORK *self) {
	assert(self);
	assert(!network_isOpen(self));
	assert(self->address);

	_network_open(self);

	/* create socket */
	self->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (self->socket == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to open socket", strerror(errno), "check your netowrk stack");
		return;
	}

	/* timeout */
	struct timeval timeout;
	timeout.tv_sec = self->timeout;
	timeout.tv_usec = 0;
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}
	if (setsockopt (self->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set send-timeout", strerror(errno), NULL);
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
			_network_error(self, NETWORK_ERROR_INFORMATION, "unable to resolve address", strerror(errno), NULL);
			return;
		}
		memcpy((char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(self->port);

	int c = connect(self->socket, (struct sockaddr *)&server, sizeof(server));
	if (c == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_CONNECTION, "unable to connect to server", strerror(errno), NULL);
		return;
	}

	self->connect = TRUE;
}

void network_close(NETWORK *self) {
	assert(self);
	assert(self->socket);

	/* close socket */
#ifdef SYSTEM_OS_TYPE_WINDOWS
	WSACleanup();
	closesocket(self->socket);
#else
	close(self->socket);
#endif
	
	self->socket = 0;

	self->connect = FALSE;
}

BOOL network_isOpen(NETWORK *self) {
	assert(self);

	return self->connect;
}

void network_serverOpen(NETWORK *self) {
	assert(self);
	assert(self->port > 0);
	assert(self->timeout > 0);
	assert(self->queue > 0);
	assert(!network_isOpen(self));

	_network_open(self);

	/* catch ctrl-c */
//	signal(SIGINT, active_server_quit_server);                /* CTRL-C */
//	signal(SIGPIPE, active_server_quit_client);               /* lost connection */
#ifdef SYSTEM_OS_TYPE_UNIX
//	signal(SIGPIPE, SIG_IGN);                   /* ignore SIGPIPE */
#endif

	/* create socket */
	self->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (self->socket == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to open socket", strerror(errno), "check your netowrk stack");
		return;
	}

	/* reuse socket */
	int iSetOption = 1;
	if (setsockopt(self->socket, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set port-reusement", strerror(errno), NULL);
		return;
	}

	/* timeout */
	struct timeval timeout;
	timeout.tv_sec = self->timeout;
	timeout.tv_usec = 0;
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}
	if (setsockopt (self->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set send-timeout", strerror(errno), NULL);
		return;
	}

	/* bind port to socket */
	struct sockaddr_in server;
    memset(&server, 0, sizeof (server));        /* clean structure */

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(self->port);

	int b = bind(self->socket, (struct sockaddr *)&server, sizeof(server));
	if (b == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_BIND, "unable to bind port", strerror(errno), NULL);
		return;
	}

	int l = listen(self->socket, self->queue);
	if (l == NETWORK_SOCKET_ERROR) {
		_network_error(self, NETWORK_ERROR_LISTEN, "unable to listen", strerror(errno), NULL);
		return;
	}
}

void network_serverAccept(NETWORK *self, NETWORK_ACCEPT_CALLBACK callback, void *object) {
	assert(self);
	assert(callback);

	NETWORK_SOCKET client_socket;
	struct sockaddr_in client_address;
	socklen_t client_length;

	while (1) {
		client_length = sizeof(client_address);
		client_socket = accept(self->socket, (struct sockaddr *)&client_address, &client_length);
		if (client_socket == NETWORK_SOCKET_ERROR) {
			_network_error(self, NETWORK_ERROR_ACCEPT, "unable to connect to client", strerror(errno), "check your connction");
			return;
		}

		/* create client */
		network_client_init(self);
		network_client_setSocket(self, client_socket);
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_address.sin_addr), str, INET_ADDRSTRLEN);
		network_client_setAddress(self, str);
		network_client_setPort(self, ntohs(client_address.sin_port));

		/* call callback */
		callback(self, object);

		/* close & free client object */
		network_client_close(self);
		network_client_free(self);
	}
}

char *network_readString(NETWORK *self) {
	assert(self);
	assert(self->client);

	NETWORK_SOCKET socket = network_client_getSocket(self);
	char buf[NETWORK_BUFFER_SIZE + 1];
	int rc;
	size_t len = 1;
	char *tmp = (char *)malloc(len);
	tmp[0] = '\0';

	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading", strerror(errno), "check the server-client-communication");
			return NULL;
		}
		buf[rc] = '\0';

		len += rc;
		tmp = (char *)realloc(tmp, len);
		strcat(tmp, buf);

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}

	char *str = string_trim(tmp);
	free(tmp);
	return str;
}

int network_readNumber(NETWORK *self) {
	assert(self);
	assert(self->client);

	NETWORK_SOCKET socket = network_client_getSocket(self);
	char buf[NETWORK_BUFFER_SIZE + 1];
	int rc;

	rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0);
	if (rc < 0) {
		_network_error(self, NETWORK_ERROR_READ, "error while reading", strerror(errno), "check the server-client-communication");
		return -1;
	}
	if (rc == NETWORK_BUFFER_SIZE) {
		_network_error(self, NETWORK_ERROR_READ, "read string has wrong size", "number too long", "check the server-client-communication");
		return -1;
	}
	
	buf[rc] = '\0';

	char *tmp = string_trim(buf);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is empty", "check the server-client-communication");
		return -1;
	}
	if (!number_isInteger(tmp)) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is not a valid number", "check the server-client-communication");
		free(tmp);
		return -1;
	}

	int num = number_toInteger(tmp);
	free(tmp);

	return num;
}

void network_client_init(NETWORK *self) {
	assert(self);
	assert(!self->client);

	NETWORK_CLIENT *client = (NETWORK_CLIENT *)malloc(sizeof(NETWORK_CLIENT));
	if (!client) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	memset(client, '\0', sizeof(NETWORK_CLIENT));

	self->client = client;
}

void network_client_setSocket(NETWORK *self, NETWORK_SOCKET socket) {
	assert(self);
	assert(self->client);

	self->client->socket = socket;
}

NETWORK_SOCKET network_client_getSocket(NETWORK *self) {
	assert(self);
	assert(self->client);

	return self->client->socket;
}

void network_client_setAddress(NETWORK *self, const char *address) {
	assert(self);
	assert(self->client);
	assert(!self->client->address);

	self->client->address = strdup(address);
}

char *network_client_getAddress(NETWORK *self) {
	assert(self);
	assert(self->client);

	return strdup(self->client->address);
}

void network_client_setPort(NETWORK *self, NETWORK_PORT port) {
	assert(self);
	assert(self->client);

	self->client->port = port;
}

NETWORK_PORT network_client_getPort(NETWORK *self) {
	assert(self);
	assert(self->client);

	return self->client->port;
}

void network_client_close(NETWORK *self) {
	assert(self);
	assert(self->client);

#ifdef SYSTEM_OS_TYPE_WINDOWS
		WSACleanup();
		closesocket(self->client->socket);
#else
		close(self->client->socket);
#endif
		self->client->socket = 0;
}

void network_client_free(NETWORK *self) {
	assert(self);
	assert(self->client);

	self->client->socket = 0;
	if(self->client->address) {
		free(self->client->address);
		self->client->address = NULL;
	}
	self->client->port = 0;

	free(self->client);
	self->client = NULL;
}
