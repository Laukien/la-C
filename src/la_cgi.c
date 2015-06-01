#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <la_string.h>
#include "la_cgi.h"
#include "la_url.h"

#define CGI_PROTOCOL "HTTP/1.1"
#define CGI_TYPE "text/html"
#define CGI_ENCODING "UTF-8"
#define CGI_LINE_SIZE 1024
#define CGI_SERVER "MyServer"
#define CGI_LIB "la-C"

struct la_cgi {
	char *header_protocol;
	char *header_server;
	char *header_type;
	char *header_encoding;
	PARAMETER *parameter;
};


CGI *cgi_new() {
	CGI *self = (CGI*)malloc(sizeof(CGI));
	if (!self) {
		fprintf(stderr, "\ndynamic memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	self->header_protocol = strdup(CGI_PROTOCOL);
	self->header_server = strdup(CGI_SERVER);
	self->header_type = strdup(CGI_TYPE);
	self->header_encoding = strdup(CGI_ENCODING);

	char *query = cgi_getQueryString();
	if (query) {
		self->parameter = cgi_queryToParameter(query);
		free(query);
	} else {
		self->parameter = parameter_new();
	}

	return self;
}

void cgi_free(CGI *self) {
	assert(self);

	free(self->header_protocol);
	self->header_protocol = NULL;

	free(self->header_server);
	self->header_server = NULL;

	free(self->header_type);
	self->header_type = NULL;

	free(self->header_encoding);
	self->header_encoding = NULL;

	parameter_free(self->parameter);
	self->parameter = NULL;

	free(self);
	self = NULL;
}

void cgi_setHeaderProtocol(CGI *self, const char *str) {
	assert(self);
	assert(str);

	free(self->header_protocol);
	self->header_protocol = strdup(str);
}

char *cgi_getHeaderProtocol(CGI *self) {
	assert(self);

	return strdup(self->header_protocol);
}

void cgi_setHeaderType(CGI *self, const char *str) {
	assert(self);
	assert(str);

	free(self->header_type);
	self->header_type = strdup(str);
}

char *cgi_getHeaderType(CGI *self) {
	assert(self);

	return strdup(self->header_type);
}

void cgi_setHeaderEncoding(CGI *self, const char *str) {
	assert(self);
	assert(str);

	free(self->header_encoding);
	self->header_encoding = strdup(str);
}

char  *cgi_getHeaderEncoding(CGI *self) {
	assert(self);

	return strdup(self->header_encoding);
}

void cgi_setHeaderServer(CGI *self, const char *str) {
	assert(self);
	assert(str);

	free(self->header_server);
	self->header_server = strdup(str);
}

char *cgi_getHeaderServer(CGI *self) {
	assert(self);

	return strdup(self->header_server);
}

char *cgi_getQueryValue(CGI *self, const char *key) {
	assert(self);
	assert(key);

	return parameter_get(self->parameter, key);
}

void cgi_sendHeaderFields(CGI *self) {
	assert(self);

	time_t t;
	struct tm *timeinfo;
	time(&t);
	timeinfo = localtime(&t);
	char *date = string_trim(asctime(timeinfo));
	printf("Server: %s\r\n", self->header_server);
	printf("X-Powered-By: %s\r\n", CGI_LIB);
	printf("Date: %s\r\n", date);
	printf("Last-Modified: %s\r\n", date);
	free(date);
}

void cgi_sendHeaderContent(CGI *self) {
	assert(self);

	printf("%s 200 OK\n", self->header_protocol);
	cgi_sendHeaderFields(self);
	printf("Content-Type: %s;charset=%s\r\n", self->header_type, self->header_encoding);
	printf("\r\n");
}

void cgi_sendHeaderCode(CGI *self, int id, const char *msg) {
	assert(self);

	printf("%s %d %s\r\n", self->header_server, id, msg);
	cgi_sendHeaderFields(self);
}

void cgi_sendHeaderRedirect(CGI *self, const char *url) {
	assert(self);

	cgi_sendHeaderCode(self, 301, "Moved Permanently");
	printf("Location: %s\r\n", url);
	printf("Content-Type: %s;charset=%s\r\n", self->header_type, self->header_encoding);
	printf("\r\n");

	printf("<html>\r\n");
	printf("\t<head>\r\n");
	printf("\t\t<title>Moved</title>\r\n");
	printf("\t</head>\r\n");
	printf("\t<body>\r\n");
	printf("\t\t<h1>Moved</h1>\r\n");
	printf("\t\t<p>This page has moved to <a href=\"%s\">%s</a>.</p>\r\n", url, url);
	printf("\t</body>\r\n");
	printf("</html>\r\n");
}

void cgi_sendFile(CGI *self, const char *filename) {
	assert(self);

	char line[CGI_LINE_SIZE];
	FILE *file;
	file = fopen(filename, "r");
	if (file) {
		while (fgets(line, CGI_LINE_SIZE, file)) {
			printf("%s", line);
		}
		fclose(file);
	} else {
		printf("Error: unable to read file \"%s\"\n", filename);
	}
}
void cgi_flush(CGI *self) {
	assert(self);

	fflush(stdout);
}

char *cgi_getQueryString() {
	char *env = getenv("QUERY_STRING");
	if (env) return strdup(env);
	else return NULL;
}

char *cgi_getRemoteUser() {
	char *env = getenv("REMOTE_USER");
	if (env) return strdup(env);
	else return NULL;
}
