#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "la_number.h"
#include "la_string.h"
#include "la_url.h"

struct la_url {
	char *protcol;
	char *host;
	int port;
	char *path;
	char *file;
	char *query;
};

URL *url_new() {
	URL *self = (URL *)malloc(sizeof(URL));
	self->protcol = NULL;
	self->host = NULL;
	self->port = -1;
	self->path = NULL;
	self->file = NULL;
	self->query = NULL;

	return self;
}

void url_free(URL *self) {
	assert(self);

	url_reset(self);

	free(self);
	self = NULL;
}

void url_reset(URL *self) {
	assert(self);

	if (self->protcol) {
		free(self->protcol);
		self->protcol = NULL;
	}
	if (self->host) {
		free(self->host);
		self->host = NULL;
	}
	self->port = -1;
	if (self->path) {
		free(self->path);
		self->path = NULL;
	}
	if (self->file) {
		free(self->file);
		self->file = NULL;
	}
	if (self->query) {
		free(self->query);
		self->query = NULL;
	}
}

void url_parse(URL *self, const char *url) {
	assert(self);
	assert(url);

	url_reset(self);

	char *tmp;
	size_t len;
	char *start, *stop;

	/* protocol */
	start = (char *)url;
	stop = strstr(start, "://");
	if (stop) {
		len = stop - start;
		tmp = (char *)malloc(len + 1);
		memcpy(tmp, start, len);
		tmp[len] = '\0';
		self->protcol = string_trim(tmp);
		free(tmp);
	}

	/* host */
	if (self->protcol) start = stop + 3;
	stop = strchr(start, '/');
	if (!stop) {
		stop = (char *)url + strlen(url) - 1;
	}
	len = stop - start;
	tmp = (char *)malloc(len + 1);
	memcpy(tmp, start, len);
	tmp[len] = '\0';

	/* port */
	start = strrchr(tmp, ':');
	if (start) {
		++start;
		stop = tmp + strlen(tmp);
		len = stop - start;
		char *port = (char *)malloc(len + 1);
		memcpy(port, start, len);
		port[len] = '\0';
		if (number_isUnsignedInteger(port)) {
			self->port = number_toUnsignedInteger(port);
		} else {
			self->port = -1;
		}
		free(port);

		--start;
		start[0] = '\0';
		self->host = strdup(tmp);

		free(tmp);
	} else {
		self->host = tmp;
		if (strcmp(self->protcol, "http") == 0) self->port = 80;
		else if (strcmp(self->protcol, "https") == 0) self->port = 443;
		else if (strcmp(self->protcol, "ftp") == 0) self->port = 21;
		else self->port = -1;
	}

	/* path */
	if (self->protcol) {
		start = (char *)url + strlen(self->protcol) + 3;
		start = strchr(start, '/');
		if (!start) {
			self->path = strdup("/");
			return;
		}
	} else start = (char *)url;
	tmp = strdup(start);

	/* query */
	start = strrchr(tmp, '?');
	if (start) {
		stop = start - strlen(tmp);
		self->query = strdup(start + 1);
		start[0] = '\0';
	}

	/* file */
	stop = strrchr(tmp, '.');
	if (stop) {
		start = strrchr(tmp, '/');
		if (start < stop) {
			self->file = strdup(start + 1);
			start[0] = '\0';
		}
	}

	if (strlen(tmp)) {
		/* remove slashes */
		while (strlen(tmp) > 0 && tmp[strlen(tmp) - 1] == '/') {
			tmp[strlen(tmp) - 1] = '\0';
		}
		self->path = strdup(tmp);
	} else {
		self->path = strdup("/");
	}

	free(tmp);
}

char *url_getProtocol(URL *self) {
	assert(self);
	
	if (self->protcol) return strdup(self->protcol);
	else return NULL;
}

char *url_getHost(URL *self) {
	assert(self);

	if (self->host) return strdup(self->host);
	else return NULL;
}

int url_getPort(URL *self) {
	assert(self);

	return self->port;
}

char *url_getResource(URL *self) {
	assert(self);

	if (!self->path) return NULL;

	if (self->file) {
		size_t len;
		char *res;
		if (strlen(self->path) == 1) {
			len = 1 + strlen(self->file);
			res = (char *)malloc(len + 1);
			sprintf(res, "/%s", self->file);
		} else {
			len = strlen(self->path) + 1 + strlen(self->file);
			res = (char *)malloc(len + 1);
			sprintf(res, "%s/%s", self->path, self->file);
		}
		return res;
	} return strdup(self->path);
}

char *url_getPath(URL *self) {
	assert(self);

	if (self->path) return strdup(self->path);
	else return NULL;
}

char *url_getFile(URL *self) {
	assert(self);

	if (self->file) return strdup(self->file);
	else return NULL;
}

char *url_getQuery(URL *self) {
	assert(self);

	if (self->query) return strdup(self->query);
	else return NULL;
}

BOOL url_isSafeCharacter(char chr) {
	return (
		(chr >= '0' && chr <= '9')
		||
		(chr >= 'A' && chr <= 'Z')
		||
		(chr >= 'a' && chr <= 'z')
	);
}

char *url_decode(const char *src) {
	char *dst = (char *) malloc(strlen(src) + 1);
	char *sav = dst;
	char a, b;
	while (*src) {
		if ((*src == '%') && ((a = src[1]) && (b = src[2])) && (isxdigit(a) && isxdigit(b))) {
			if (a >= 'a') a -= 'a'-'A';
			if (a >= 'A') a -= ('A' - 10);
			else a -= '0';
			if (b >= 'a') b -= 'a'-'A';
			if (b >= 'A') b -= ('A' - 10);
			else b -= '0';
			*dst++ = 16*a+b;
			src+=3;
		} else {
			*dst++ = *src++;
		}
	}
	*dst++ = '\0';

	char *res = strdup(sav);
	free(sav);

	return res;
}

char *url_encode(const char *src) {
	unsigned int idx = 0;
	size_t len = strlen(src) * 3 + 1;
	char *dst = (char *)malloc(len);
	memset(dst, '\0', len);
	char enc[3];
	unsigned int i;
	for (i = 0; i < strlen(src); ++i) {
		if (url_isSafeCharacter(src[i])) {
			dst[idx++] = src[i];
		} else {
			sprintf(enc, "%2x", src[i]);
			dst[idx++] = '%';
			strcpy(dst + idx, enc);
			idx += 2;
		}
		if (idx >= len) break;
	}

	char *res = strdup(dst);
	free(dst);

	return res;
}

PARAMETER *cgi_queryToParameter(const char *query) {
	PARAMETER *param = parameter_new();
	char *key;
	char *value;
	char *tmp;
	char *str = strdup(query);

	char *token = strtok(str, "&");
	while (token) {
		key = token;
		while (*token && (*token != '=')) token++;
		if (*token) {
			*(token++) = '\0';
			if (token[0] != '\0') {
				tmp = url_decode(key);
				key = string_trim(tmp);
				free(tmp);
				if (!key) continue;

				tmp = url_decode(token);
				if (!tmp) {
					free(key);
					continue;
				}
				value = string_trim(tmp);
				free(tmp);
				if (!value) {
					free(key);
					continue;
				}

				parameter_addReplace(param, key, value);
				free(value);
				free(key);
			}
		}

		token = strtok((char *)0, "&");
	}

	free(str);

	return param;
}
