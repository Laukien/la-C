#ifndef LA_CGI_H
#define LA_CGI_H

#include "la_boolean.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct la_cgi CGI;

CGI *cgi_new();
void cgi_free(CGI *self);
void cgi_setHeaderProtocol(CGI *self, const char *str);
char *cgi_getHeaderProtocol(CGI *self);
void cgi_setHeaderType(CGI *self, const char *str);
char *cgi_getHeaderType(CGI *self);
void cgi_setHeaderEncoding(CGI *self, const char *str);
char *cgi_getHeaderEncoding(CGI *self);
void cgi_setHeaderServer(CGI *self, const char *str);
char *cgi_getHeaderServer(CGI *self);
char *cgi_getQueryValue(CGI *self, const char *key);
void cgi_sendHeaderFields(CGI *self);
void cgi_sendHeaderContent(CGI *self);
void cgi_sendHeaderCode(CGI *self, int id, const char *msg);
void cgi_sendHeaderRedirect(CGI *self, const char *url);
void cgi_sendFile(CGI *self, const char *filename);

void cgi_flush();
char *cgi_getQueryString();
char *cgi_getRemoteUser();

#ifdef __cplusplus
}
#endif

#endif
