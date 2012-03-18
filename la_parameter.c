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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  new
 *  Description:  
 * =====================================================================================
 */
LA_PARAMETER *parameter_new () {
	LA_PARAMETER *param = (LA_PARAMETER*) malloc ( sizeof(LA_PARAMETER) );
	if ( param==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	
	param->key = NULL;
	param->value = NULL;
	param->next = NULL;

	return param;
}		/* -----  end of function new  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  add
 *  Description:  
 * =====================================================================================
 */
void parameter_add (LA_PARAMETER *param, char *key, char *value) {
	LA_PARAMETER *node = (LA_PARAMETER*) malloc (sizeof(LA_PARAMETER) );
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
}		/* -----  end of function add  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get
 *  Description:  
 * =====================================================================================
 */
char *parameter_get (LA_PARAMETER *param, char *key) {
	LA_PARAMETER *node = param;
	do {
		node = node->next;

		if ( strcmp(node->key, key) == 0 ) return node->value;
	} while (node->next != NULL);

	return NULL;
}		/* -----  end of function get  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  size
 *  Description:  
 * =====================================================================================
 */
unsigned int size (LA_PARAMETER *param) {
	if ( param->next == NULL) return 0;
	LA_PARAMETER *node = param;
	unsigned int i=0;
	do {
		node = node->next;
		i++;
	} while (node->next != NULL);

	return i;
}		/* -----  end of function size  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  free
 *  Description:  
 * =====================================================================================
 */
void parameter_free (LA_PARAMETER *param) {
	LA_PARAMETER *node;
	LA_PARAMETER *next;

	node = param;
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
	param = NULL;
}		/* -----  end of function free  ----- */
