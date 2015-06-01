#ifndef LA_URL_H
#define LA_URL_H

#include "la_boolean.h"
#include "la_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct la_url URL;

URL *url_new();
void url_free(URL *self);
void url_reset(URL *self);
void url_parse(URL *self, const char *url);
char *url_getUrl(URL *self);
char *url_getProtocol(URL *self);
char *url_getHost(URL *self);
int url_getPort(URL *self);
char *url_getResource(URL *self);
char *url_getPath(URL *self);
char *url_getFile(URL *self);
char *url_getQuery(URL *self);

BOOL url_isSafeCharacter(char chr);
char *url_decode(const char *src);
char *url_encode(const char *src);
PARAMETER *cgi_queryToParameter(const char *query);

#ifdef __cplusplus
}
#endif

#endif
