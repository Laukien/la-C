/*
 * =====================================================================================
 *
 *       Filename:  la_network.h
 *    Description:  basic network communication functions
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_NETWORK_H
#define LA_NETWORK_H

#define NETWORK_ERROR_INIT 1
#define NETWORK_ERROR_QUIT 2
#define NETWORK_ERROR_SYSTEM 3
#define NETWORK_ERROR_OPTION 4
#define NETWORK_ERROR_INFORMATION 5
#define NETWORK_ERROR_CONNECTION 6
#define NETWORK_ERROR_BIND 7
#define NETWORK_ERROR_LISTEN 8
#define NETWORK_ERROR_ACCEPT 9
#define NETWORK_ERROR_READ 10
#define NETWORK_ERROR_WRITE 11

#define NETWORK_BUFFER_SIZE 1024                /* 1 KB */
#define NETWORK_DATA_SIZE 1024 * 1024           /* 1 MB */

#include "la_system.h"
#include "la_exception.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct la_network NETWORK;
typedef struct la_network_accept NETWORK_ACCEPT;
typedef struct la_network_data NETWORK_DATA;

#ifdef SYSTEM_OS_TYPE_WINDOWS
	#define NETWORK_SOCKET_ERROR SOCKET_ERROR
	#define INET_ADDRSTRLEN 16
	typedef int NETWORK_SOCKET;
	typedef int NETWORK_SEND;
	typedef int NETWORK_RECV;
	typedef int socklen_t;
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
void network_setPort(NETWORK *self, NETWORK_PORT port);
void network_setTimeout(NETWORK *self, int timeout);
void network_setQueue(NETWORK *self, int queue);
void network_open(NETWORK *self);
BOOL network_isOpen(NETWORK *self);
void network_close(NETWORK *self);
void network_writeString(NETWORK *self, const char *str);
char *network_readString(NETWORK *self);
void network_writeNumber(NETWORK *self, int num);
int network_readNumber(NETWORK *self);
void network_writeStatus(NETWORK *self, BOOL status);
BOOL network_readStatus(NETWORK *self);
void network_writeFile(NETWORK *self, const char *filename);
void network_readFile(NETWORK *self, const char *filename);
void network_writeData(NETWORK *self);
void network_readData(NETWORK *self);

void network_callAccept(NETWORK *self, NETWORK_ACCEPT_CALLBACK callback, void *object);
NETWORK_SOCKET network_getAcceptSocket(NETWORK *self);
char *network_getAcceptAddress(NETWORK *self);
NETWORK_PORT network_getAcceptPort(NETWORK *self);
BOOL network_isAccept(NETWORK *self);

void network_initData(NETWORK *self);
void network_freeData(NETWORK *self);
void network_setDataLimit(NETWORK *self, size_t limit);
size_t network_getDataLimit(NETWORK *self);
size_t network_getDataSize(NETWORK *self);
void network_setDataBinary(NETWORK *self, const char *content, size_t size);
char *network_getDataBinary(NETWORK *self);
void network_setDataString(NETWORK *self, const char *content);
char *network_getDataString(NETWORK *self);

#ifdef __cplusplus
}

#include <string>

namespace la {
	namespace exception {
		class NetworkException : public Exception {
			public:
				NetworkException() : Exception() {};
				NetworkException(EXCEPTION *e) : Exception(e) {};
				NetworkException(int id, const std::string &message) : Exception(id, message) {};
				NetworkException(int id, const std::string &message, const std::string &cause, const std::string &action) : Exception(id, message, cause, action) {};
		};
	}

	namespace network {
		class Network {
			private:
				NETWORK *obj;
				EXCEPTION *err;
			public:
				Network();
				~Network();
				void setAddress(const std::string &adr);
				void setPort(NETWORK_PORT port);
				void setTimeout(int timeout);
				void setQueue(int queue);
				void open() throw(la::exception::NetworkException);
				bool isOpen();
				void close() throw(la::exception::NetworkException);
				void writeString(const std::string &str) throw(la::exception::NetworkException);
				std::string readString() throw(la::exception::NetworkException);
				void writeNumber(int num) throw(la::exception::NetworkException);
				int readNumber() throw(la::exception::NetworkException);
				void writeStatus(bool status) throw(la::exception::NetworkException);
				bool readStatus() throw(la::exception::NetworkException);
				void writeFile(const std::string &filename) throw(la::exception::NetworkException);
				void readFile(const std::string &filename) throw(la::exception::NetworkException);
				void writeData() throw(la::exception::NetworkException);
				void readData() throw(la::exception::NetworkException);

				void callAccept(NETWORK_ACCEPT_CALLBACK callback, void *object) throw(la::exception::NetworkException);
				NETWORK_SOCKET getAcceptSocket();
				std::string getAcceptAddress();
				NETWORK_PORT getAcceptPort();
				bool isAccept();

				void initData();
				void freeData();
				void setDataLimit(size_t limit);
				size_t getDataLimit();
				size_t getDataSize();
				void setDataBinary(const char *content, size_t size);
				char *getDataBinary();
				void setDataString(const std::string &content);
				std::string getDataString();
		};
	}
}
#endif
#endif
