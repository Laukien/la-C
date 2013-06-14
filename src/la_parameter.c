/*
 * =====================================================================================
 *
 *       Filename:  la_parameter.c
 *
 *    Description:  store for all parameters
 *
 *        Version:  1.0
 *        Created:  11/14/2011 10:25:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Laukien (slaukien), software@laukien.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "la_parameter.h"
#include "la_string.h"

struct la_parameter {
	char *key;
	char *value;
	struct la_parameter *next;
};

PARAMETER *parameter_getNode (PARAMETER *self, unsigned int index) {
	PARAMETER *node = self;
	if (self->next == NULL) return NULL;       /* check if parameter-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (index == count) {
			return node;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

PARAMETER *parameter_new () {
	PARAMETER *param = (PARAMETER*) malloc ( sizeof(PARAMETER) );
	if ( param==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	
	param->key = NULL;
	param->value = NULL;
	param->next = NULL;

	return param;
}

void parameter_add (PARAMETER *param, const char *key, const char *value) {
	PARAMETER *node = (PARAMETER*) malloc (sizeof(PARAMETER) );
	if ( node==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	/* init next */
	node->next = NULL;

	/* create key */
	node->key = (char *) malloc (strlen(key)+1);
	if ( node->key==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->key, key);

	/* create value */
	node->value = (char *) malloc (strlen(value)+1);
	if ( node->value==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->value, value);

	/* set pointer */
	unsigned int size = parameter_size(param);
	if ( size == 0) {
		node->next = param->next;
		param->next = node;
	} else {
		PARAMETER *last = parameter_getNode(param, size - 1);
		last->next = node;
	}
}

void parameter_remove (PARAMETER *self, const char *key) {
	PARAMETER *nodeSelf, *nodePrev, *nodeNext;
	unsigned int index = parameter_getIndexByKey(self, key);

	nodeSelf = parameter_getNode(self, index);
	if (nodeSelf == NULL) return;
	
	unsigned int size = parameter_size(self);
	if (size <= 1) {
		parameter_reset(self);
		return;
	}

	if (index == 0) {                           /* first element */
		nodeNext = nodeSelf->next;
		self->next = nodeNext;
	} else if (index == (size - 1)) {           /* last element */
		nodePrev = parameter_getNode(self, index - 1);
		nodePrev->next = NULL;
	} else {
		nodePrev = parameter_getNode(self, index - 1);
		nodeNext = nodeSelf->next;
		nodePrev->next = nodeNext;
	}
	/* free key */
	free(nodeSelf->key);
	nodeSelf->key = NULL;
	/* free value */
	free(nodeSelf->value);
	nodeSelf->value = NULL;
	/* free node */
	free(nodeSelf);
	nodeSelf = NULL;
}

PARAMETER *parameter_getByIndex (PARAMETER *self, unsigned int index) {
	PARAMETER *node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* init parameter */
			PARAMETER *param;
			param = (PARAMETER *) malloc(sizeof(PARAMETER));
			param->next = NULL;

			/* copy key */
			param->key = (char *) malloc (strlen(node->key)+1);
			strcpy(param->key, node->key);

			/* copy value */
			param->value = (char *) malloc (strlen(node->value)+1);
			strcpy(param->value, node->value);

			return param;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

char *parameter_getKeyByIndex (PARAMETER *self, unsigned int index) {
	PARAMETER *node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* copy key */
			char *key;
			key = (char *) malloc (strlen(node->key)+1);
			strcpy(key, node->key);
			return key;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

char *parameter_getValueByIndex (PARAMETER *self, unsigned int index) {
	PARAMETER *node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (index == count) {
			/* copy value */
			char *value;
			value = (char *) malloc (strlen(node->value)+1);
			strcpy(value, node->value);
			return value;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

unsigned int parameter_getIndexByKey (PARAMETER *self, const char *key) {
	PARAMETER *node = self;
    if (self->next == NULL) return 0;           /* check if list-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (strcmp(node->key, key) == 0) return count;

		++count;
	} while (node->next != NULL);

	return 0;
}

char *parameter_getValueByKey (PARAMETER *param, const char *key) {
	PARAMETER *node = param;
	if (param->next == NULL) return NULL;       /* check if parameter-set is empty */

	do {
		node = node->next;

		if ( strcmp(node->key, key) == 0 ) {
			/* copy value */
			char *value;
			value = (char *) malloc (strlen(node->value)+1);
			strcpy(value, node->value);
			return value;
		}
	} while (node->next != NULL);

	return NULL;
}

char *parameter_get (PARAMETER *param, const char *key) {
	return parameter_getValueByKey(param, key);
}

unsigned int parameter_size (PARAMETER *param) {
	if ( param->next == NULL) return 0;

	PARAMETER *node = param;
	unsigned int count = 0;

	do {
		node = node->next;
//		printf ( "NODE: %s->%s\n", node->key, node->value );
		++count;
	} while (node->next != NULL);

	return count;
}


void parameter_free (PARAMETER *param) {
	PARAMETER *node;
	PARAMETER *next;

	node = param;                               /* get the first parameter */
	while (node->next != NULL) {
		next = node->next;
//		node->next=NULL;
		if (node->key != NULL ) free(node->key);
		if (node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if (node->key != NULL ) free(node->key);
	if (node->value != NULL) free(node->value);
	if (node != NULL) free(node);               /* free only if there are items */
	node = NULL;
}

void parameter_reset (PARAMETER *param) {
	PARAMETER *node;
	PARAMETER *next;

	node = param->next;                         /* get the next parameter */
	if (node == NULL) return;

	while (node->next != NULL) {
		next = node->next;
//		node->next=NULL;
		if ( node->key != NULL ) free(node->key);
		if ( node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if ( node->key != NULL ) free(node->key);
	if ( node->value != NULL) free(node->value);
	free(node);
	node = NULL;

	param->next = NULL;
}

void parameter_show(PARAMETER *self) {
	unsigned int i;
	PARAMETER *param;
	for (i = 0; i < parameter_size(self); ++i) {
		param = parameter_getByIndex(self, i);
		printf ( "%s = %s\n", param->key, param->value );
		free(param);
	}
}

int parameter_loadFromFile(PARAMETER *param, const char *filename) {
	parameter_reset(param);

	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL) return -1;

	unsigned int count = 0;
	char *key;
	char *value;
	char line[PARAMETER_KEY_SIZE + 1 + PARAMETER_VALUE_SIZE + 2 + 1];
	int len = sizeof(line)/sizeof(line[0]);
//	memset(line, '\0', len);
	while (fgets(line, len, file) != NULL) {
		/* correct string */
		char *c = string_replaceFirst(line, "#", "\0");
		char *t = string_trim(c);
		free(c);

		if ( t == NULL ) continue;
		if ( strlen(t) == 0) {
			free(t);
			continue;
		}

		strcpy(line, t);
		free(t);

		/* split key-value */
		char *sep = strchr(line, '=');
        if (sep == NULL) continue;              /* no separator */
		if ((sep - line) > PARAMETER_KEY_SIZE) continue;
		if ((strlen(line) - (sep - line) - 1) > PARAMETER_VALUE_SIZE) continue;
		sep[0] = '\0';                          /* make the separator th the key-end */
		key = string_trim(line);
        ++sep;                                  /* move one byte right */
		value = string_trim(sep);

		parameter_add(param, key, value);
		++count;

		free(value);
		free(key);
//		memset(line, '\0', len);
	}
	fclose(file);

	return count;
}

int parameter_saveToFile(PARAMETER *param, const char *filename) {
	if (param->next == NULL) return 0;          /* no parameters to save */

	PARAMETER *node;
	node = param;

	unsigned int count = 0;
	FILE *file;
	file = fopen(filename, "w");
	if (file == NULL) return -1;

	do {
		node = node->next;
		if (!string_isEmpty(node->key) && !string_isEmpty(node->value)) {
			fprintf ( file, "%s=%s\n", node->key, node->value );
			++count;
		}
	 } while (node->next != NULL);

	fclose(file);

	return count;
}

#ifdef __cplusplus
namespace la {
	parameter::parameter() {
		this->obj = parameter_new();
	}

	parameter::parameter(PARAMETER *obj) {
		this->obj = obj;
	}

	parameter::~parameter() {
		parameter_free(this->obj);
	}

	void parameter::add(const std::string &key, const std::string &value) {
		parameter_add(this->obj, key.c_str(), value.c_str());
	}

	void parameter::remove(const std::string &key) {
		parameter_remove(this->obj, key.c_str());
	}

	void parameter::reset() {
		parameter_reset(this->obj);
	}

	parameter parameter::getByIndex(unsigned int index) {
		PARAMETER *p;
		p = parameter_getByIndex(this->obj, index);
		if (p == NULL)
			return NULL;

		return parameter(p);
	}

	std::string parameter::getKeyByIndex(unsigned int index) {
		char *tmp = parameter_getKeyByIndex(this->obj, index);
		if (tmp == NULL)
			return std::string();
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	std::string parameter::getValueByIndex(unsigned int index) {
		char *tmp = parameter_getValueByIndex(this->obj, index);
		if (tmp == NULL)
			return std::string();
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	unsigned int parameter::getIndexByKey(const std::string &key) {
		return parameter_getIndexByKey(this->obj, key.c_str());
	}

	std::string parameter::getValueByKey(const std::string &key) {
		char *tmp = parameter_getValueByKey(this->obj, key.c_str());
		if (tmp == NULL)
			return std::string();
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	std::string parameter::get(const std::string &key) {
		char *tmp = parameter_get(this->obj, key.c_str());
		if (tmp == NULL)
			return std::string();
		std::string res = std::string(tmp);
		free(tmp);

		return res;
	}

	unsigned int parameter::size() {
		return parameter_size(this->obj);
	}

	void parameter::show() {
		parameter_show(this->obj);
	}

	void parameter::loadFromFile(const std::string &filename) {
		parameter_loadFromFile(this->obj, filename.c_str());
	}

	void parameter::saveToFile(const std::string &filename) {
		parameter_saveToFile(this->obj, filename.c_str());
	}
}
#endif
