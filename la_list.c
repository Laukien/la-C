/*
 * =====================================================================================
 *
 *       Filename:  la_list.c
 *
 *    Description:  store for elements in a list
 *
 *        Version:  1.0
 *        Created:  11/08/2012 10:25:55 PM
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

#include "la_list.h"
#include "la_string.h"

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

void list_add (LIST *self, const char *value) {
	LIST *node = (LIST*) malloc (sizeof(LIST) );
	if ( node==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	/* create value */
	node->value = (char *) malloc (strlen(value)+1);
	if ( node->value==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->value, value);

	/* set pointer */
	node->next = self->next;
	self->next = node;
}

char *list_get (LIST *self, unsigned int index) {
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

unsigned int list_size (LIST *self) {
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


void list_free (LIST *self) {
	LIST *node;
	LIST *next;

	node = self;                               /* get the first list */
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
}

void list_reset (LIST *self) {
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

int list_loadFromFile(LIST *self, const char *filename) {
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
		char *c = string_replaceFirst(line, "#", "\0");
		char *t = string_trim(c);
		free(c);

		if ( t == NULL || strlen(t) == 0) {
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
