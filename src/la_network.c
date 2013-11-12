#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
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
	NETWORK_ACCEPT *accept;
	NETWORK_DATA *data;
	BOOL connect;
	char *address;
	int port;
	int timeout;
	int queue;
};

struct la_network_accept {
	int socket;
	char *address;
	int port;
};

struct la_network_data {
	size_t limit;
	size_t size;
	char *content;
};

/* define internal accept functions */
void network_initAccept(NETWORK *self);
void network_setAcceptSocket(NETWORK *self, NETWORK_SOCKET socket);
void network_setAcceptAddress(NETWORK *self, const char *address);
void network_setAcceptPort(NETWORK *self, NETWORK_PORT port);
void network_closeAccept(NETWORK *self);
void network_freeAccept(NETWORK *self);

void _network_error(NETWORK *self, int id, const char *message, const char *cause, const char *action) {
	if (self && self->exception) {
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

void _network_signal_quit(int sig) {
	_network_error(NULL, NETWORK_ERROR_QUIT, "process has been quit", NULL, NULL);
}

void _network_signal_lost(int sig) {
	message_info("lost connection");
}

void _network_open_client(NETWORK *self) {
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

void _network_open_server(NETWORK *self) {
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
	/*
	if (setsockopt (self->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
		_network_error(self, NETWORK_ERROR_OPTION, "unable to set recv-timeout", strerror(errno), NULL);
		return;
	}*/
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

NETWORK *network_new() {
	NETWORK *self = (NETWORK *)malloc(sizeof(NETWORK));
	if (!self) {
		message_error("unable to get memory");
	}

	memset(self, 0, sizeof(NETWORK));
	self->socket = 0;
	self->accept = NULL;
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

	if (self->accept) {
		network_closeAccept(self);
		network_freeAccept(self);
	}
	if (self->data) {
		network_freeData(self);
	}

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

void network_open(NETWORK *self) {
	assert(self);
	assert(!self->socket);
	assert(self->port > 0);
	assert(self->timeout > 0);
	assert(self->queue > 0);
	assert(!network_isOpen(self));

	/* debug */
	message_debug("network_open");

	/* catch signals */
    signal(SIGINT, _network_signal_quit);       /* CTRL-C */
#ifdef SYSTEM_OS_TYPE_UNIX
    signal(SIGPIPE, _network_signal_lost);      /* lost connection */
//	signal(SIGPIPE, SIG_IGN);                   /* ignore SIGPIPE */
#endif

#ifdef SYSTEM_OS_TYPE_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2 ,0), &wsaData)) {
		int errCode = WSAGetLastError();
		LPSTR errString = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errCode, 0, (LPSTR)&errString, 0, 0);
		_network_error(self, NETWORK_ERROR_INIT, "unable to init winsock", errString, "check up Windows network stack");
		LocalFree(errString);
		return;
	}
#endif

	if (self->address) {
		_network_open_client(self);
	} else {
		_network_open_server(self);
	}

}

void network_close(NETWORK *self) {
	assert(self);
	assert(self->socket);

	/* debug */
	message_debug("network_close");

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

void network_writeString(NETWORK *self, const char *str) {
	assert(self);

	/* debug */
	message_debug("network_writeString(%s)", str);

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
	size_t size = strlen(str);

	/* add line break */
	char *tmp = (char *)malloc(size + 2 + 1);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	strcpy(tmp, str);
	strcat(tmp, "\r\n");
	size += 2;                                  /* add 2 characters for line break */

	size_t sent = 0;
	int rc;
	int len;
	do {
		if ((size - sent) > NETWORK_BUFFER_SIZE) {
			len = NETWORK_BUFFER_SIZE;
		} else {
			len = size - sent;
		}
		rc = send(socket, tmp + sent, len, 0);
		if (rc == -1) {
			_network_error(self, NETWORK_ERROR_WRITE, "unable to send data", strerror(errno), "check the network stack");
			return;
		}

		sent += rc;
	} while (sent < size);
	free(tmp);
}

char *network_readString(NETWORK *self) {
	assert(self);

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
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
//	if (!str) printf ( "NULL\n" );
	free(tmp);

	/* debug */
	message_debug("network_readString(%s)", str);

	return str;
}

void network_writeNumber(NETWORK *self, int num) {
	assert(self);

	/* debug */
	message_debug("network_writeNumber(%d)", num);

	char *str = number_integerToString(num);
	network_writeString(self, str);
	free(str);
}

int network_readNumber(NETWORK *self) {
	assert(self);

	char *tmp = network_readString(self);
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

	/* debug */
	message_debug("network_readNumber(%d)", num);

	return num;
}

void network_writeStatus(NETWORK *self, BOOL status) {
	assert(self);

	/* debug */
	message_debug("network_writeStatus(%s)", status ? "TRUE" : "FALSE");

	char *str = boolean_toString(status);
	network_writeString(self, str);
	free(str);
}

BOOL network_readStatus(NETWORK *self) {
	assert(self);

	char *tmp = network_readString(self);
	if (!tmp) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is empty", "check the server-client-communication");
		return FALSE;
	}
	if (!boolean_isBoolean(tmp)) {
		_network_error(self, NETWORK_ERROR_READ, "read string is wrong", "string is not a valid boolean value", "check the server-client-communication");
		free(tmp);
		return FALSE;
	}

	BOOL status = boolean_toBoolean(tmp);
	free(tmp);

	/* debug */
	message_debug("network_readStatus(%s)", status ? "TRUE": "FALSE");

	return status;
}

void network_writeFile(NETWORK *self, const char *filename) {
	assert(self);

	/* debug */
	message_debug("network_writeFile(%s)", filename);

	/* create and open file */
	int fd;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	fd = open(filename, O_RDONLY|O_BINARY);
#else
	fd = open(filename, O_RDONLY);
#endif
	if (fd == -1) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to open file", strerror(errno), "check the file permission");
		return;
	}

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
	/* write file */
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	int rc;
	while ((rc = read(fd, buf, NETWORK_BUFFER_SIZE))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading file", strerror(errno), "check the file permission");
			return;
		}

		len += rc;
		send(socket, buf, rc, 0);

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}
	close(fd);
}

void network_readFile(NETWORK *self, const char *filename) {
	assert(self);

	/* debug */
	message_debug("network_readFile(%s)", filename);

	/* create and open file */
	int fd;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	fd = open(filename, O_WRONLY|O_CREAT|O_BINARY, 0644);
#else
	fd = open(filename, O_WRONLY|O_CREAT, 0644);
#endif
	if (fd == -1) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to open file", strerror(errno), "check the permission");
		return;
	}

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
	/* write file */
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	int rc;
	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
		if (rc < 0) {
			_network_error(self, NETWORK_ERROR_READ, "error while reading", strerror(errno), "check the server-client-communication");
			return;
		}

		len += rc;
		write(fd, buf, rc);

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
	}
	close(fd);
}

void network_writeData(NETWORK *self) {
	assert(self);
	assert(self->data);

	/* debug */
	message_debug("network_writeData");

	if (self->data->size > self->data->limit) {
		_network_error(self, NETWORK_ERROR_INIT, "size to huge", "limit is less than size", "change the limit or check the size");
		return;
	}

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
	size_t size = self->data->size;
	char *data = self->data->content;
	size_t sent = 0;
	int rc;
	int len;
	do {
		if ((size - sent) > NETWORK_BUFFER_SIZE) {
			len = NETWORK_BUFFER_SIZE;
		} else {
			len = size - sent;
		}
		rc = send(socket, data + sent, len, 0);
		if (rc == -1) {
			_network_error(self, NETWORK_ERROR_WRITE, "unable to send data", strerror(errno), "check the network stack");
			return;
		}

		sent += rc;
	} while (sent < size);
}

void network_readData(NETWORK *self) {
	assert(self);

	/* debug */
	message_debug("network_readData");

	NETWORK_SOCKET socket = self->accept ? self->accept->socket : self->socket;
	size_t len = 0;
	char buf[NETWORK_BUFFER_SIZE];
	unsigned long count = 0;
	char *content = (char *)malloc(1);
	int rc;
	while ((rc = recv(socket, buf, NETWORK_BUFFER_SIZE, 0))) {
//		content = (char *)realloc(content, (count + 1) * NETWORK_BUFFER_SIZE);
		content = (char *)realloc(content, count * NETWORK_BUFFER_SIZE + rc);
		if (!content) {
			_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the network communicaton");
			return;
		}
		memcpy(content + (count * NETWORK_BUFFER_SIZE), buf, rc);

		len += rc;

		if (rc < NETWORK_BUFFER_SIZE) {
			break;
		}
		++count;
	}

	network_initData(self);
	network_setDataLimit(self, len);
	network_setDataBinary(self, content, len);
	free(content);
}

void network_callAccept(NETWORK *self, NETWORK_ACCEPT_CALLBACK callback, void *object) {
	assert(self);
	assert(self->socket);
	assert(!self->accept);
	assert(callback);

	/* debug */
	message_debug("network_callAccept");

	NETWORK_SOCKET client_socket;
	struct sockaddr_in client_address;
	socklen_t client_length;

	while (1) {
		client_length = sizeof(client_address);
		client_socket = accept(self->socket, (struct sockaddr *)&client_address, &client_length);
		if (client_socket == NETWORK_SOCKET_ERROR) {
			_network_error(self, NETWORK_ERROR_ACCEPT, "unable to connect to client", strerror(errno), "check your connection");
			return;
		}

		/* create client socket */
		network_initAccept(self);
		network_setAcceptSocket(self, client_socket);
		char str[INET_ADDRSTRLEN];
#ifdef SYSTEM_OS_TYPE_WINDOWS
		strcpy(str, inet_ntoa(client_address.sin_addr)); /* deprecated */
#else
		inet_ntop(AF_INET, &(client_address.sin_addr), str, INET_ADDRSTRLEN);
#endif
		network_setAcceptAddress(self, str);
		network_setAcceptPort(self, ntohs(client_address.sin_port));

		/* debug */
		message_debug("accept(%s)", str);

		/* call callback */
		callback(self, object);

		/* close & free client object */
		network_closeAccept(self);
		network_freeAccept(self);
	}
}

void network_initAccept(NETWORK *self) {
	assert(self);
	assert(!self->accept);

	NETWORK_ACCEPT *client = (NETWORK_ACCEPT *)malloc(sizeof(NETWORK_ACCEPT));
	if (!client) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	memset(client, '\0', sizeof(NETWORK_ACCEPT));

	self->accept = client;
	self->connect = TRUE;
}

void network_setAcceptSocket(NETWORK *self, NETWORK_SOCKET socket) {
	assert(self);
	assert(self->accept);

	self->accept->socket = socket;
}

NETWORK_SOCKET network_getAcceptSocket(NETWORK *self) {
	assert(self);
	assert(self->accept);

	return self->accept->socket;
}

void network_setAcceptAddress(NETWORK *self, const char *address) {
	assert(self);
	assert(self->accept);
	assert(!self->accept->address);

	self->accept->address = strdup(address);
}

char *network_getAcceptAddress(NETWORK *self) {
	assert(self);
	assert(self->accept);

	return strdup(self->accept->address);
}

void network_setAcceptPort(NETWORK *self, NETWORK_PORT port) {
	assert(self);
	assert(self->accept);

	self->accept->port = port;
}

NETWORK_PORT network_getAcceptPort(NETWORK *self) {
	assert(self);
	assert(self->accept);

	return self->accept->port;
}

BOOL network_isAccept(NETWORK *self) {
	assert(self);

	return self->accept ? TRUE : FALSE;
}

void network_closeAccept(NETWORK *self) {
	assert(self);
	assert(self->accept);

#ifdef SYSTEM_OS_TYPE_WINDOWS
		WSACleanup();
		closesocket(self->accept->socket);
#else
		close(self->accept->socket);
#endif
		self->accept->socket = 0;
		self->connect = FALSE;
}

void network_freeAccept(NETWORK *self) {
	assert(self);
	assert(self->accept);

	self->accept->socket = 0;
	if(self->accept->address) {
		free(self->accept->address);
		self->accept->address = NULL;
	}
	self->accept->port = 0;

	free(self->accept);
	self->accept = NULL;
}

void network_initData(NETWORK *self) {
	assert(self);

	/* free memory */
	if (self->data) {
		network_freeData(self);
	}

	NETWORK_DATA *data = (NETWORK_DATA *)malloc(sizeof(NETWORK_DATA));
	if (!data) {
		_network_error(self, NETWORK_ERROR_SYSTEM, "unable to get memory", strerror(errno), "check the system");
		return;
	}
	
	data->limit = NETWORK_DATA_SIZE;
	data->size = 0;
	data->content = NULL;

	self->data = data;
}

void network_freeData(NETWORK *self) {
	assert(self);
	assert(self->data);

	self->data->limit = NETWORK_DATA_SIZE;
	self->data->size = 0;
	if (self->data->content) {
		free(self->data->content);
		self->data->content = NULL;
	}

	free(self->data);
	self->data = NULL;

}

void network_setDataLimit(NETWORK *self, size_t limit) {
	assert(self);
	assert(self->data);

	self->data->limit = limit;
}

size_t network_getDataLimit(NETWORK *self) {
	assert(self);
	assert(self->data);

	return self->data->limit;
}

size_t network_getDataSize(NETWORK *self) {
	assert(self);
	assert(self->data);

	return self->data->size;
}

void network_setDataBinary(NETWORK *self, const char *content, size_t size) {
	assert(self);
	assert(self->data);

	self->data->size = size;

	/* check, free and set content */
	if (self->data->content) {
		free(self->data->content);
		self->data->content = NULL;
	}
	self->data->content = (char *)malloc(size);
	if (!self->data->content) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check system memory");
		self->data->size = 0;
		return;
	}
	memcpy(self->data->content, content, size);
}

char *network_getDataBinary(NETWORK *self) {
	assert(self);
	assert(self->data);

	/* check, free and set content */
	if (self->data->size == 0 || !self->data->content) {
		return NULL;
	}

	char *content = (char *)malloc(self->data->size);
	if (!content) {
		_network_error(self, NETWORK_ERROR_INIT, "unable to get memory", strerror(errno), "check system memory");
		return NULL;
	}
	memcpy(content, self->data->content, self->data->size);

	return content;
}

void network_setDataString(NETWORK *self, const char *content) {
	assert(self);

	network_setDataBinary(self, content, strlen(content));
}

char *network_getDataString(NETWORK *self) {
	assert(self);

	size_t size = network_getDataSize(self);
	char *str = network_getDataBinary(self);
	str = (char *)realloc(str, size + 1);
	str[size] = '\0';

	return str;
}

#ifdef __cplusplus
namespace la {
	network::network() {
		this->obj = network_new();
	}

	network::~network() {
		network_free(this->obj);
	}

	void network::setException(EXCEPTION *e) {
		network_setException(this->obj, e);
	}

	void network::setAddress(const std::string &adr) {
		network_setAddress(this->obj, adr.c_str());
	}

	void network::setPort(NETWORK_PORT port) {
		network_setPort(this->obj, port);
	}

	void network::setTimeout(int timeout) {
		network_setTimeout(this->obj, timeout);
	}

	void network::setQueue(int queue) {
		network_setQueue(this->obj, queue);
	}

	void network::open() {
		network_open(this->obj);
	}

	bool network::isOpen() {
		return network_isOpen(this->obj);
	}

	void network::close() {
		network_close(this->obj);
	}

	void network::writeString(const std::string &str) {
		network_writeString(this->obj, str.c_str());
	}

	std::string network::readString() {
		char *tmp = network_readString(this->obj);
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	void network::writeNumber(int num) {
		return network_writeNumber(this->obj, num);
	}

	int network::readNumber() {
		return network_readNumber(this->obj);
	}

	void network::writeStatus(bool status) {
		network_writeStatus(this->obj, status);
	}

	bool network::readStatus() {
		return network_readStatus(this->obj);
	}

	void network::writeFile(const std::string &filename) {
		network_writeFile(this->obj, filename.c_str());
	}

	void network::readFile(const std::string &filename) {
		network_readFile(this->obj, filename.c_str());
	}

	void network::writeData() {
		network_writeData(this->obj);
	}

	void network::readData() {
		network_readData(this->obj);
	}

	void network::callAccept(NETWORK_ACCEPT_CALLBACK callback, void *object) {
		network_callAccept(this->obj, callback, object);
	}

	NETWORK_SOCKET network::getAcceptSocket() {
		return network_getAcceptSocket(this->obj);
	}

	std::string network::getAcceptAddress() {
		char *tmp = network_getAcceptAddress(this->obj);
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	NETWORK_PORT network::getAcceptPort() {
		return network_getAcceptPort(this->obj);
	}

	bool network::isAccept() {
		return network_isAccept(this->obj);
	}

	void network::initData() {
		return network_initData(this->obj);
	}

	void network::freeData() {
		return network_freeData(this->obj);
	}

	void network::setDataLimit(size_t limit) {
		return network_setDataLimit(this->obj, limit);
	}

	size_t network::getDataLimit() {
		return network_getDataLimit(this->obj);
	}

	size_t network::getDataSize() {
		return network_getDataSize(this->obj);
	}

	void network::setDataBinary(const char *content, size_t size) {
		network_setDataBinary(this->obj, content, size);
	}

	char *network::getDataBinary() {
		return network_getDataBinary(this->obj);
	}

	void network::setDataString(const std::string &content) {
		network_setDataString(this->obj, content.c_str());
	}

	std::string network::getDataString() {
		char *tmp = network_getDataString(this->obj);
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}
}
#endif
