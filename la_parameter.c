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
	node->next = param->next;
	param->next = node;
}

char *parameter_get (PARAMETER *param, const char *key) {
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

unsigned int parameter_size (PARAMETER *param) {
	if ( param->next == NULL) return 0;

	PARAMETER *node = param;
	unsigned int count = 0;

	do {
		node = node->next;
		printf ( "NODE: %s->%s\n", node->key, node->value );
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
		if ( node->key != NULL ) free(node->key);
		if ( node->value != NULL) free(node->value);
		free(node);
		node = next;
	}
	
	if ( node->key != NULL ) free(node->key);
	if ( node->value != NULL) free(node->value);
	free(node);
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

		if ( t == NULL || strlen(t) == 0) {
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
