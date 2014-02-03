/*
 * =====================================================================================
 *
 *       Filename:  la_list.c
 *    Description:  handle (string) objects in a linked list
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "la_boolean.h"
#include "la_list.h"
#include "la_string.h"

struct la_list {
	char *value;
	struct la_list *next;
};

LIST *list_getNode(LIST *self, unsigned int index) {
	assert(self);

	LIST *node = self;
	if (self->next == NULL) return NULL;       /* check if list-set is empty */

	unsigned int count = 0;
	do {
		node = node->next;

		if (index == count) {
			return node;
		} else ++count;
	} while (node->next != NULL);

	return NULL;
}

LIST *list_new () {
	LIST *self = (LIST*) malloc ( sizeof(LIST) );
	if ( self==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	
	self->value = NULL;
	self->next = NULL;

	return self;
}

void list_add(LIST *self, const char *value) {
	assert(self);

	LIST *node = (LIST*) malloc (sizeof(LIST) );
	if ( node==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	/* init next */
	node->next = NULL;

	/* create value */
	node->value = (char *) malloc (strlen(value)+1);
	if ( node->value==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->value, value);

	/* set pointer */
	unsigned int size = list_size(self);
	if ( size == 0) {
		node->next = self->next;
		self->next = node;
	} else {
		LIST *last = list_getNode(self, size - 1);
		last->next = node;
	}
}

void list_addUnique(LIST *self, const char *value) {
	assert(self);

	if (!list_exists(self, value))
		list_add(self, value);
}

BOOL list_exists(LIST *self, const char *value) {
	assert(self);

	LIST *node = self;
	if (self->next == NULL)
		return FALSE;

	do {
		node = node->next;

		if (strcmp(value, node->value) == 0)
			return TRUE;
	} while (node->next != NULL);

	return FALSE;
}

void list_remove(LIST *self, unsigned int index) {
	assert(self);

	LIST *nodeSelf, *nodePrev, *nodeNext;

	nodeSelf = list_getNode(self, index);
	if (nodeSelf == NULL) return;
	
	unsigned int size = list_size(self);
	if (size <= 1) {
		list_reset(self);
		return;
	}

	if (index == 0) {                           /* first element */
		nodeNext = nodeSelf->next;
		self->next = nodeNext;
	} else if (index == (size - 1)) {           /* last element */
		nodePrev = list_getNode(self, index - 1);
		nodePrev->next = NULL;
	} else {
		nodePrev = list_getNode(self, index - 1);
		nodeNext = nodeSelf->next;
		nodePrev->next = nodeNext;
	}
	free(nodeSelf->value);
	nodeSelf->value = NULL;
	free(nodeSelf);
	nodeSelf = NULL;
}

void list_swap(LIST *self, unsigned int index1, unsigned int index2) {
	assert(self);

	unsigned int len = list_size(self);
	assert(index1 >= 0 && index1 < len && index2 >= 0 && index2 < len);

	if (index1 == index2) return;

//	LIST *tmp11 = index1 == 0 ? self : list_getNode(self, index1-1);
//	LIST *tmp12 = index2 == 0 ? self : list_getNode(self, index2-1);
//	LIST *tmp10 = tmp11->next;
//
//	LIST *tmp21 = list_getNode(self, index1);
//	LIST *tmp22 = list_getNode(self, index2);
//	LIST *tmp20 = tmp22->next;
//
//	tmp11->next = tmp12->next;
//	tmp12->next = tmp10;
//
//	tmp22->next = tmp21->next;
//	tmp21->next = tmp20;

	LIST *node1, *node2;
	char *tmp;

	node1 = list_getNode(self, index1);
	node2 = list_getNode(self, index2);

	tmp = node1->value;
	node1->value = node2->value;
	node2->value = tmp;
}

void list_sort(LIST *self) {
	assert(self);

	BOOL swap;
	unsigned int len = list_size(self);
	unsigned int i;

	/* only two or more items */
	if (len <= 1) {
		return;
	}

	do {
		swap = FALSE;
		for (i = 0; i < len - 1; ++i) {
			if (strcmp(list_getNode(self, i)->value, list_getNode(self, i + 1)->value) > 0) {
				swap = TRUE;
				list_swap(self, i, i + 1);
			}
		}
	} while (swap);
}

char *list_get(LIST *self, unsigned int index) {
	assert(self);

	LIST *node = self;
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

unsigned int list_size(LIST *self) {
	assert(self);

	if ( self->next == NULL) return 0;

	LIST *node = self;
	unsigned int count = 0;

	do {
		node = node->next;
//		printf ( "NODE: %s->%s\n", node->key, node->value );
		++count;
	} while (node->next != NULL);

	return count;
}


void list_free(LIST *self) {
	assert(self);

	LIST *node;
	LIST *next;

	node = self;                               /* get the first list */
	while (node->next != NULL) {
		next = node->next;
//		node->next=NULL;
		if (node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if (node->value != NULL) free(node->value);
    if (node != NULL) free(node);               /* free only if there are items */
	node = NULL;
}

void list_reset(LIST *self) {
	assert(self);

	LIST *node;
	LIST *next;

	node = self->next;                         /* get the next list */
	if (node == NULL) return;

	while (node->next != NULL) {
		next = node->next;
//		node->next=NULL;
		if ( node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if ( node->value != NULL) free(node->value);
	free(node);
	node = NULL;

	self->next = NULL;
}

void list_show(LIST *self) {
	assert(self);

	unsigned int i;
	char *str;
	for (i = 0; i < list_size(self); ++i) {
		str = list_get(self, i);
		puts(str);
		free(str);
	}
}

int list_loadFromFile(LIST *self, const char *filename) {
	assert(self);

	list_reset(self);

	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL) return -1;

	unsigned int count = 0;
	char line[LIST_VALUE_SIZE + 2 + 1];
	int len = sizeof(line)/sizeof(line[0]);
//	memset(line, '\0', len);
	while (fgets(line, len, file) != NULL) {
		/* correct string */
//		char *c = string_replaceFirst(line, "#", "\0");
		char *t = string_trim(line);
		if (t == NULL) continue;
		if (t[0] == '#') {
			free(t);
			continue;
		}

		strcpy(line, t);
		free(t);

		list_add(self, line);
		++count;

//		memset(line, '\0', len);
	}
	fclose(file);

	return count;
}

int list_saveToFile(LIST *self, const char *filename) {
	assert(self);

	if (self->next == NULL) return 0;          /* no lists to save */

	LIST *node;
	node = self;

	unsigned int count = 0;
	FILE *file;
	file = fopen(filename, "w");
	if (file == NULL) return -1;

	do {
		node = node->next;
		if (!string_isEmpty(node->value)) {
			fprintf ( file, "%s\n", node->value );
			++count;
		}
	 } while (node->next != NULL);

	fclose(file);

	return count;
}

#ifdef __cplusplus
namespace la {
	list::list() {
		this->obj = list_new();
	}

	list::list(LIST *obj) {
		this->obj = obj;
	}

	list::~list() {
		list_free(this->obj);
	}

	void list::add(const std::string &value) {
		list_add(this->obj, value.c_str());
	}

	void list::addUnique(const std::string &value) {
		list_addUnique(this->obj, value.c_str());
	}

	bool list::exists(const std::string &value) {
		return list_exists(this->obj, value.c_str());
	}

	void list::remove(unsigned int index) {
		list_remove(this->obj, index);
	}

	void list::swap(unsigned int index1, unsigned int index2) {
		list_swap(this->obj, index1, index2);
	}

	void list::sort() {
		list_sort(this->obj);
	}

	void list::reset() {
		list_reset(this->obj);
	}

	std::string list::get(unsigned int index) {
		char *value = list_get(this->obj, index);
		std::string res = std::string(value);
		free(value);

		return res;
	}

	unsigned int list::size() {
		return list_size(this->obj);
	}

	void list::show() {
		list_show(this->obj);
	}

	void list::loadFromFile(const std::string &filename) {
		list_loadFromFile(this->obj, filename.c_str());
	}

	void list::saveToFile(const std::string &filename) {
		list_saveToFile(this->obj, filename.c_str());
	}
}
#endif
