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
#include <assert.h>

#include "la_parameter.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  newParameter
 *  Description:  
 * =====================================================================================
 */
PARAMETER *newParameter ()
{
	PARAMETER *param = (PARAMETER*)malloc ( sizeof(PARAMETER) );
	if ( param==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	
	param->key = NULL;
	param->value = NULL;
	param->next = NULL;

	return param;
}		/* -----  end of function newParameter  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  addParameter
 *  Description:  
 * =====================================================================================
 */
void addParameter (PARAMETER *param, char *key, char *value)
{
	assert(param != NULL);

	struct parameter *node = (PARAMETER*)malloc (sizeof(PARAMETER) );
	if ( node==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}

	/* create key */
	node->key = malloc (strlen(key)+1);
	if ( node->key==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->key, key);

	/* create value */
	node->value = malloc (strlen(value)+1);
	if ( node->value==NULL ) {
		fprintf ( stderr, "\ndynamic memory allocation failed\n" );
		exit (EXIT_FAILURE);
	}
	strcpy(node->value, value);

	/* set pointer */
	node->next = param->next;
	param->next = node;
}		/* -----  end of function addParameter  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getParameterValue
 *  Description:  
 * =====================================================================================
 */
char *getParameterValue (PARAMETER *param, char *key)
{
	assert(param != NULL);

	PARAMETER *node = param;
	do {
		node = node->next;

		if ( strcmp(node->key, key) == 0 ) return node->value;
	} while (node->next != NULL);

	return NULL;
}		/* -----  end of function getParameterValue  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getParameterSize
 *  Description:  
 * =====================================================================================
 */
int getParameterSize (PARAMETER *param)
{
	assert(param != NULL);

	if ( param->next == NULL) return 0;
	PARAMETER *node = param;
	int i=0;
	do {
		node = node->next;
		i++;
	} while (node->next != NULL);

	return i;
}		/* -----  end of function getParameterSize  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  destroyParameter
 *  Description:  
 * =====================================================================================
 */
void destroyParameter (PARAMETER *param)
{
	assert(param != NULL);

	PARAMETER *node;
	PARAMETER *next;

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
}		/* -----  end of function destroyParameter  ----- */
