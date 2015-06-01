#include <stdio.h>
#include <stdlib.h>
#include "la_url.h"

int main(void) {
	URL *url = url_new();
	url_parse(url, "http://doamin.tld:8080/path/file.html?query&key=value");
	char *protocol = url_getProtocol(url);
	char *host = url_getHost(url);
	int port = url_getPort(url);
	char *resource = url_getResource(url);
	char *path = url_getPath(url);
	char *file = url_getFile(url);
	char *query = url_getQuery(url);

	printf ( "PROTOCOL: %s\n", protocol );
	printf ( "HOST: %s\n", host );
	printf ( "PORT: %d\n", port );
	printf ( "RESOURCE: %s\n", resource );
	printf ( "PATH: %s\n", path );
	printf ( "FILE: %s\n", file );
	printf ( "QUERY: %s\n", query );

	char *str = "Dieses ist ein Test-String!";
	char *enc = url_encode(str);
	char *dec = url_decode(enc);
	printf ( "STRING: %s\n", str );
	printf ( "ENCODE: %s\n", enc );
	printf ( "DECODE: %s\n", dec );

	if (dec) free(dec);
	if (enc) free(enc);
	if (query) free(query);
	if (file) free(file);
	if (path) free(path);
	if (resource) free(resource);
	if (host) free(host);
	if (protocol) free(protocol);
	url_free(url);

	return EXIT_SUCCESS;
}
