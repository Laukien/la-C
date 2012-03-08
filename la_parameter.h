/*
 * =====================================================================================
 *
 *       Filename:  la_parameter.h
 *
 *    Description:  
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

#ifndef LA_PARAMETER_H
#define LA_PARAMETER_H

struct parameter {
	char *key;
	char *value;
	struct parameter *next;
};

typedef struct parameter PARAMETER;

PARAMETER *newParameter();
void addParameter (PARAMETER *param, char *key, char *value);
void removeParameter (PARAMETER *param, char *key);
void clearParameter (PARAMETER *param);
void destroyParameter (PARAMETER *param);
char *getParameterValue (PARAMETER *param, char *key);
int getParameterSize (PARAMETER *param);
void loadParameterFromFile(PARAMETER *param, char *filename);
void saveParameterToFile(PARAMETER *param, char *filename);

#endif

